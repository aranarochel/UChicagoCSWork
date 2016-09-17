/*
 *  chidb - a didactic relational database management system
 *
 *  SQL -> DBM Code Generator
 *
 */

/*
 *  Copyright (c) 2009-2015, The University of Chicago
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or withsend
 *  modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  - Neither the name of The University of Chicago nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software withsend specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY send OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <chidb/chidb.h>
#include <chisql/chisql.h>
#include "dbm.h"
#include "util.h"


int chidb_get_tables(list_t tables, chisql_statement_t *sql_statement);
int chidb_get_sra_tables(list_t tables, SRA_t *s);
int chidb_get_create_tables(list_t tables, Create_t *cre);
int chidb_get_select_columns(list_t column_names, Expression_t *exp_list);
int load_schema(chidb *db, npage_t nroot);

int chidb_stmt_seed(chidb_stmt *stmt, list_t *ops, list_t *tables, list_t *names);
int chidb_stmt_insert_op(chidb_stmt *stmt, list_t *ops, chidb_dbm_op_t *new_op);
int chidb_stmt_select_project(chidb_stmt *stmt, list_t tables);
int chidb_stmt_project_expand(chidb_stmt *stmt, SRA_t *sra, list_t tables);

int chidb_stmt_select_star_expand(SRA_Project_t *sra_project, list_t names, list_t names2);
int chidb_count_select_columns(int *ncols, Expression_t *exp_list);

/* Step 1 schema loading is in api.c, steps 2-5 contained in here */

//Creates and allocates an op
chidb_dbm_op_t *chidb_make_op(opcode_t code, int32_t p1, int32_t p2, int32_t p3, char *p4)
{
    chidb_dbm_op_t *ret = malloc(sizeof(chidb_dbm_op_t));
    ret->opcode = code;
    ret->p1 = p1;
    ret->p2 = p2;
    ret->p3 = p3;
    ret->p4 = p4;
    return ret;
}

//Standardized function for error checking
int chidb_stmt_check(chidb_stmt *stmt, chisql_statement_t *sql_stmt, list_t table_names)
{   
    // checks for table existence
    list_init(&table_names);
    list_t col_names;
    list_init(&col_names);

    chidb_get_tables(table_names, sql_stmt);

    list_iterator_start(&table_names);
    while (list_iterator_hasnext(&table_names))
    {
        char *next = (char *)list_iterator_next(&table_names);
        if (chidb_table_exists(stmt->db->schemas, next) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    list_iterator_stop(&table_names);

    list_iterator_start(&col_names);
    while (list_iterator_hasnext(&col_names))
    {
        char *next = (char *)list_iterator_next(&col_names);
        if (chidb_column_exists(stmt->db->schemas, list_get_at(&table_names, 0), next) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    list_iterator_stop(&col_names);

    list_destroy(&table_names);
    list_destroy(&col_names);


    return CHIDB_OK;

}

/********************** Step 4: Create Table Code Generation ***********************/

int chidb_stmt_create(chidb_stmt *stmt, chisql_statement_t *sql_stmt)
{
    int opnum = 0;
    int nOps;
    int i;

    int ret = chidb_table_exists(stmt->db->schemas, sql_stmt->stmt.create->table->name);
    if(ret == CHIDB_OK)
        return CHIDB_EINVALIDSQL;

    // clipping the semicolon
    (sql_stmt->text)[strlen(sql_stmt->text)-1] = '\0';
    


    // the type field of record is set to "table"... for now...
    chidb_dbm_op_t ops[] = {
            {Op_Integer, 1, 0, 0, NULL},
            {Op_OpenWrite, 0, 0, 5, NULL},
            {Op_CreateTable, 4, 0, 0, NULL},
            {Op_String, 5, 1, 0, "table"}, // will need to change to support indicies
            {Op_String,strlen(sql_stmt->stmt.create->table->name),2,0,sql_stmt->stmt.create->table->name},
            {Op_String,strlen(sql_stmt->stmt.create->table->name),3,0,sql_stmt->stmt.create->table->name},
            {Op_String,(int32_t)strlen(sql_stmt->text),5,0,sql_stmt->text},
            {Op_MakeRecord,1,5,6,NULL},
            {Op_Integer,(int32_t)list_size(&(stmt->db->schemas))+1,7,0,NULL},
            {Op_Insert,0,6,7,NULL},
            {Op_Close,0,0,0,NULL}
    };

    nOps = 11; //sizeof(ops) / sizeof(chidb_dbm_op_t);

    stmt->sql = sql_stmt;
    stmt->nOps = nOps;

    for(i=0; i < nOps; i++)
        chidb_stmt_set_op(stmt, &ops[i], opnum++);

    stmt->db->need_refresh = 1;

    return CHIDB_OK;
}

/********************** Step 3: Insert Code Generation ***********************/

int chidb_stmt_insert(chidb_stmt *stmt, chisql_statement_t *sql_stmt)
{
    // Unpacking some variables so they are easier to access later

    char *table_name = sql_stmt->stmt.insert->table_name;
    // NOTE!!! This is NOT the names of columns in the table! Rather, columns specified for the insert!

    Literal_t *values = sql_stmt->stmt.insert->values;
    int numVals = 0; // This will be filled in as we error check (to do only one pass its faster)

    //------------------Error Checking first----------------------

    // Check if table name exists --assuming for now that inserting into all columns
    if(chidb_table_exists(stmt->db->schemas, table_name) != CHIDB_OK)
    {
        fprintf(stderr, "%s\n", "Table does not exist!");
        return CHIDB_EINVALIDSQL;
    }

    // This list will be the actual column names of the table we are inserting into
    list_t cnames;
    list_init(&cnames);
    int ret = chidb_column_names(stmt->db->schemas, table_name, &cnames);
    if(ret == CHIDB_EINVALIDSQL)
    {
        fprintf(stderr, "%s\n", "No column names!");
        return ret;
    }

    // Check that types match up
    // Iterate over each column, obtain type, and then check with the value given
    list_iterator_start(&cnames);
    while(list_iterator_hasnext(&cnames))
    {
        char *col_name = (char *)(list_iterator_next(&cnames));
        int ret = chidb_column_get_type(stmt->db->schemas, table_name, col_name);
        if(ret == CHIDB_EINVALIDSQL)
            return ret;
        else
        {
            if(values == NULL)
            {
                // If the values list runs out before the string list, something is really wrong
                return CHIDB_EINVALIDSQL;
            }
            if(values->t != ret) // Ret holds the type of the column
            {
                fprintf(stderr, "Input data type mismatch in column %s\n", col_name);
                return CHIDB_EINVALIDSQL;
            }
            numVals++;
            values = values->next;
        }
    }

    //-------------produce actual insert---------------------------
    // Create a list to store (we don't know how many ops needed yet so can't use array)
    list_t ops;
    list_init(&ops);

    // Get root page, then store it in register zero
    int root = chidb_get_root(stmt->db->schemas, table_name);
    if(root == CHIDB_EINVALIDSQL){return root;}

    chidb_dbm_op_t *first = chidb_make_op(Op_Integer, root, 0, 0, NULL); // The root page number is now in reg 0
    list_append(&ops, first); // I'm fine passing the address, because this list will only be used in this function
    chidb_dbm_op_t *second = chidb_make_op(Op_OpenWrite, 0, 0, list_size(&cnames), NULL);
    list_append(&ops, second);
    // Open write using cursor zero

    // Now create the record
    int reg = 1; // So we don't overwrite regs
    values = sql_stmt->stmt.insert->values; // Need to put this back because we ruined it before
    while(values != NULL)
    {
        if(values->t == TYPE_INT)
        {
            chidb_dbm_op_t *next = chidb_make_op(Op_Integer, values->val.ival, reg, 0, NULL);
            list_append(&ops, next);
        }
        else if(values->t == TYPE_TEXT)
        {
            chidb_dbm_op_t *next = chidb_make_op(Op_String, strlen(values->val.strval), reg, 0, values->val.strval);
            list_append(&ops, next);
        }
        // Other values are unspported by chisql

        if(reg==1) //The key slot in CHIDB needs to be null, as specified in the project spec
        {
            reg++;
            chidb_dbm_op_t * next = chidb_make_op(Op_Null, 0, reg, 0, NULL);
            list_append(&ops, next);
            reg++;
        }
        else
        {
            reg++;
        }

        values = values->next;
    }

    // Create record from r1 through (r1+n-1), store in r2
    // *NOTE: the primary key will always be first
    chidb_dbm_op_t *make = chidb_make_op(Op_MakeRecord,2,reg-2,reg, NULL);
    list_append(&ops, make);

    // Cursor 0, record stored at reg+1, the first one is the primary key
    chidb_dbm_op_t *insert = chidb_make_op(Op_Insert,0,reg,1, NULL);
    list_append(&ops, insert);
    // Close Cursor 0
    chidb_dbm_op_t *close = chidb_make_op(Op_Close,0,0,0,NULL);
    list_append(&ops, close);

    // Finally, transfer the list of ops into an array of ops, and fix it all up in the actual stmt
    int numOps = list_size(&ops);
    int i;
    for(i = 0; i < numOps; i++)
    {
        chidb_dbm_op_t *next = (chidb_dbm_op_t *)list_get_at(&ops, i);
        chidb_stmt_set_op(stmt, next, i);
        // There seems to be a leak having to do with set_op but I can't figure out how to plug it...
    }

    //Free everything! 
    chidb_dbm_op_t *op_free;
    while(!list_empty(&ops))
    {
        op_free = (chidb_dbm_op_t *)list_fetch(&ops);
        free(op_free->p4);
        free(op_free);
    }

    list_destroy(&ops);
    list_destroy(&cnames);

    return CHIDB_OK;
}

/********************** Step 2: Simple Select Code Generation ***********************/

/* 
 * assumes well-formed sql queries abiding by project spec limitations.
 * note: sra_select != NULL means there is a WHERE
 *       sra_table2 != NULL means there is a NATURAL JOIN
 */
int chidb_stmt_select(chidb_stmt *stmt, chisql_statement_t *sql_stmt)
{
    // ---------------------convenience list setup-----------------------------

    list_t tnames;      // Tables we are selecting from
    list_t cnames1;     // Column names in table 1
    list_t cnames2;     // Column names in table 2 (if natural join)
    list_t snames;      // Column names that are being selected
    list_t ops;         // Chidb_ops held in here

    list_init(&tnames);
    list_init(&cnames1);
    list_init(&cnames2);
    list_init(&snames);
    list_init(&ops);


    // ------------------------sql_stmt unpacking------------------------------
    // -------------------and convenience list populating----------------------

    SRA_Project_t *sra_project = NULL;
    SRA_Select_t *sra_select = NULL;
    SRA_Table_t *sra_table1 = NULL;
    SRA_Table_t *sra_table2 = NULL;

    SRA_t *sra_next = sql_stmt->stmt.select;
    char *next_name;
    
    while(sra_next != NULL)
    {
        switch (sra_next->t)
        {
            case SRA_TABLE:
                // If natural join, both tables set there
                sra_table1 = &(sra_next->table);
                sra_next = NULL; // Table doesnt have next but we're at end

                // Check to see if the table exists
                if(chidb_table_exists(stmt->db->schemas, sra_table1->ref->table_name))
                {
                    // chilog error: table does not exist
                    fprintf(stderr, "%s\n", "esql: 333");
                    return CHIDB_EINVALIDSQL;
                }

                // Add name of table1 to tnames list
                list_append(&tnames, strdup(sra_table1->ref->table_name));

                // Populate cnames1 list with column names of table1
                chidb_column_names(stmt->db->schemas, list_get_at(&tnames, 0), &cnames1);
                break;

            case SRA_PROJECT:
                sra_project = &(sra_next->project);
                sra_next = sra_project->sra;
                break;

            case SRA_SELECT:
                sra_select = &(sra_next->select);
                sra_next = sra_select->sra;
                break;

            case SRA_NATURAL_JOIN:
                // If single table, set in SRA_TABLE case
                // We only support 2 tables being natural joined
                sra_table1 = &(sra_next->binary.sra1->table);
                sra_table2 = &(sra_next->binary.sra2->table);
                sra_next = NULL;

                // Append names of tables to tnames list
                list_append(&tnames, strdup(sra_table1->ref->table_name));
                list_append(&tnames, strdup(sra_table2->ref->table_name));

                // Check to see if tables exist
                list_iterator_start(&tnames);
                while(list_iterator_hasnext(&tnames))
                {
                    next_name = (char *)list_iterator_next(&tnames);
                    if(chidb_table_exists(stmt->db->schemas,next_name))
                    {
                        // chilog warning!! table doesn't exist
                        fprintf(stderr, "%s\n", "fuck");
                        return CHIDB_EINVALIDSQL; 
                    }

                }
                list_iterator_stop(&tnames);

                // Populate cnames1 and cnames2 list with column names of t1 and t2 respectively
                chidb_column_names(stmt->db->schemas, list_get_at(&tnames, 0), &cnames1);
                chidb_column_names(stmt->db->schemas, list_get_at(&tnames, 1), &cnames2);
                break;

            default:
                // We don't support anything else
                fprintf(stderr, "%s\n", "esql: 363");
                return CHIDB_EINVALIDSQL;
        }
    }

    // ----------select columns error checking and upstream expansion----------

    char *first_col = sra_project->expr_list->expr.term.ref->columnName;

    Expression_t *expr_next = sra_project->expr_list;
    while(expr_next != NULL)
    {
        // Update for next iteration
        expr_next = expr_next->next;
    }

    if(*first_col == '*')
    {
        // fprintf(stderr, "%s\n", "star needed expanding");
        
        // Expand the star so that no longer a special case of selecting
        chidb_stmt_select_star_expand(sra_project, cnames1, cnames2);
    }

    // Populate the select names list 
    expr_next = sra_project->expr_list;
    while(expr_next != NULL)
    {
        // Add to the select names list
        list_append(&snames, expr_next->expr.term.ref->columnName); 

        // Update for next iteration
        expr_next = expr_next->next;
    }

    // =========================== CODEGEN SECTION ============================

    // *** Initialization ***

    // Newop for clarity
    chidb_dbm_op_t *new_op;

    // Registers
    int comp_val_reg;   // Where comparison register
    int comp_col_reg;   // The column we're comparing to if there is a where
    int c1_reg;
    int c2_reg = 0;
    int comp_nj_t1_reg;
    int comp_nj_t2_reg; 
    int first_col_reg;  // Either last cursor reg + 1 or comp_nj_t2_reg + 1;

    // Instruction offsets
    int open_off; // Offset of first open read integer injection
    int rewind_off; // Offset of the last rewind insn (next jumps to rewind_off+1)
    int comp_off = 0;   // Where comparison insn (this will be rewind_off+2)
    int comp_nj_off; // Offset of first comparison thingy for natural join
    int col_off;    // Offset of when we start calling columns
    int rr_off;     // Result row instruction (next is rr_off-1)

    // Other
    npage_t root;   // Used to load in the root page
    int col_pos;    // Position of the column we want to produce
    int col_pos2;   // Position of column we want to pull out (table 2 for joining)
    int col_c_reg;  // The cursor that points to the particular cursor
    int jump_addr;  // Makes clear where we are jumping to
    int num_common_cols = 0;    // If natural joining

    // These are used only in the case of having select
    // Documentation says: column OP value. we say comp_column comp_op comp_value
    ColumnReference_t *comp_column; 
    Literal_t *comp_value;         
    enum CondType comp_op; 

    // *** If we have a where, insert the comp value at first instruction ***
    if(sra_select != NULL)
    {
        comp_column = sra_select->cond->cond.comp.expr1->expr.term.ref;
        comp_value = sra_select->cond->cond.comp.expr2->expr.term.val;
        comp_op = sra_select->cond->t;


        // Set the first register(s)
        comp_val_reg = 0;
        comp_col_reg = 1;

        switch(comp_value->t)
        {
            case TYPE_INT:
                new_op = chidb_make_op(Op_Integer,
                                       comp_value->val.ival,
                                       comp_val_reg,
                                       0,
                                       NULL);
                list_append(&ops, new_op);
                break;

            case TYPE_TEXT:
                new_op = chidb_make_op(Op_String, 
                                       strlen(comp_value->val.strval), 
                                       comp_val_reg, 
                                       0, 
                                       comp_value->val.strval);
                list_append(&ops, new_op);
                break;

            default:
                fprintf(stderr, "%s\n", "esql: 476");
                return CHIDB_EINVALIDSQL; // Not supported otherwise

        }

        // Update the offset of open
        open_off = 1;

    }
    else
    {
        // Update the offset of open read
        open_off = 0;
    }

    // *** Open cursor(s) for reading and rewind ***
    // First cursor
    if(sra_select == NULL)
        c1_reg = 0; // Gets 0 register if no where
    else
        c1_reg = 2; // Gets 2 register if column doing where on is not selected

    // Get root page of first table
    root = chidb_get_root(stmt->db->schemas, list_get_at(&tnames, 0));

    // Insert page into register we are opening the cursor on, open for reading
    list_append(&ops, chidb_make_op(Op_Integer, root, c1_reg, 0, NULL));
    list_append(&ops, chidb_make_op(Op_OpenRead, c1_reg, c1_reg, list_size(&cnames1), NULL));

    // Update rewind offset
    rewind_off = open_off + 2;

    // Second cursor (if there is a natural join)
    if(sra_table2 != NULL)
    {
        c2_reg = c1_reg + 1;

        root = chidb_get_root(stmt->db->schemas, list_get_at(&tnames,1));

        list_append(&ops, chidb_make_op(Op_Integer, root, c2_reg, 0, NULL));
        list_append(&ops, chidb_make_op(Op_OpenRead, c2_reg, c2_reg, list_size(&cnames2), NULL));
        rewind_off += 2; // Leave rewind offset at first rewind for now
    }

    // Rewind cursor(s) -- these must be updated later with close insn location
    list_append(&ops, chidb_make_op(Op_Rewind, c1_reg, 0, 0, NULL));

    if(sra_table2 != NULL)
    {
        list_append(&ops, chidb_make_op(Op_Rewind, c2_reg, 0, 0, NULL));
        rewind_off++; // Advance rewind offset to be at the last rewind
    }

    // *** Revisiting the case if we have a where ***
    if(sra_select != NULL)
    {
        // Update the comparison offset
        comp_off = rewind_off + 2;

        // Get the column position to get the column with op_key or op_column
        col_pos = chidb_column_position(&cnames1, comp_column->columnName);
        col_c_reg = c1_reg; 
        if(sra_table2 != NULL && col_pos < 0) // if not found in first table
        {
            col_pos = chidb_column_position(&cnames2, comp_column->columnName);
            col_c_reg = c2_reg;
        }
        if(col_pos < 0) // Error checking (not found in either table)
        {
            // The column trying to select (sigma) on does not exist
            fprintf(stderr, "%s\n", "esql: 548");
            return CHIDB_EINVALIDSQL;
        }

        // Add the op to grab the column
        if(col_pos == 0)
            new_op = chidb_make_op(Op_Key, col_c_reg, comp_col_reg, 0, NULL);
        else
            new_op = chidb_make_op(Op_Column, col_c_reg, col_pos, comp_col_reg, NULL);
        list_append(&ops, new_op); // Actually add

        // Add the op to make the comparison. needs to be updated with jump to next later.
        switch(comp_op)
        {
            case RA_COND_EQ:
                new_op = chidb_make_op(Op_Ne, comp_val_reg, 0, comp_col_reg, NULL);
                break;
            case RA_COND_LT:
                new_op = chidb_make_op(Op_Ge, comp_val_reg, 0, comp_col_reg, NULL);
                break;
            case RA_COND_GT:
                new_op = chidb_make_op(Op_Le, comp_val_reg, 0, comp_col_reg, NULL);
                break;
            case RA_COND_LEQ:
                new_op = chidb_make_op(Op_Gt, comp_val_reg, 0, comp_col_reg, NULL);
                break;
            case RA_COND_GEQ:
                new_op = chidb_make_op(Op_Lt, comp_val_reg, 0, comp_col_reg, NULL);
                break;
            default:
                // Our implementation of chidb does not support the other options
                fprintf(stderr, "%s\n", "esql: 579");
                return CHIDB_EINVALIDSQL;
        }
        list_append(&ops, new_op); // Actually add
    }

    // *** some natural join goes here ***
    if(sra_table2 != NULL)
    {
        // Set the offsets of where the nj comparistons start
        // These are in blocks of 3 instructions
        if(sra_select == NULL)
        {
            comp_nj_off = rewind_off + 1;
        }
        else
        {
            comp_nj_off = comp_off + 1;
        }

        // Set the registers we'll be using
        comp_nj_t1_reg = c2_reg + 1; // We share with first_col_reg
        comp_nj_t2_reg = c2_reg + 2;

        // Count number of common columns and add the ops to list
        num_common_cols = 0;
        list_iterator_start(&snames);
        while(list_iterator_hasnext(&snames))
        {
            next_name = (char *)list_iterator_next(&snames);
            col_pos = chidb_column_position(&cnames1, next_name);
            col_pos2 = chidb_column_position(&cnames2, next_name);
            if(col_pos >=0 && col_pos2 >=0)
            {
                num_common_cols++;

                // Load column from table 1
                if(col_pos == 0)
                    new_op = chidb_make_op(Op_Key, c1_reg, comp_nj_t1_reg, 0, NULL);
                else
                    new_op = chidb_make_op(Op_Column, c1_reg, col_pos, comp_nj_t1_reg, NULL);
                list_append(&ops, new_op);

                // Load column from table 2
                if(col_pos2 == 0)
                    new_op = chidb_make_op(Op_Key, c2_reg, comp_nj_t2_reg, 0, NULL);
                else
                    new_op = chidb_make_op(Op_Column, c2_reg, col_pos2, comp_nj_t2_reg, NULL);
                list_append(&ops, new_op);

                // Not equal op. Each one of these needs to be updated at end w/ jump to inner next
                list_append(&ops, chidb_make_op(Op_Ne, comp_nj_t1_reg, 0, comp_nj_t2_reg, NULL));
            }
        }
        list_iterator_stop(&snames);
    }

    // *** Column and result row ops! ***
    // Update the col_off, first_col_reg
    if(sra_select == NULL)
    {
        col_off = rewind_off + 1;
    }
    else
    {
        col_off = comp_off + 1;
    }
    // Also, if natural join, register is different
    if(sra_table2 == NULL)
    {
        first_col_reg = c1_reg + 1;
    }
    else
    {
        first_col_reg = c2_reg + 1;
    }

    list_iterator_start(&snames);
    while(list_iterator_hasnext(&snames))
    {
        fprintf(stderr, "col: %s\n", (char *)list_iterator_next(&snames));
    }
    list_iterator_stop(&snames);

    int col_reg = first_col_reg; // Next col register
    char *next_col_name;
    list_iterator_start(&snames);
    while(list_iterator_hasnext(&snames))
    {
        next_col_name = (char *)list_iterator_next(&snames);

        // Get the column position to get the column with op_key or op_column
        col_pos = chidb_column_position(&cnames1, next_col_name);
        
        col_c_reg = c1_reg;
 
        if(sra_table2 != NULL && col_pos < 0) // If not found in first table
        {
            // fprintf(stderr, "%s\n", "Considering second column\n");
            col_pos = chidb_column_position(&cnames2, next_col_name);
            col_c_reg = c2_reg;
        }
        
        if(col_pos < 0) // Error checking (not found in either table)
        {
            // fprintf(stderr, "looking for column: %s\n", next_col_name);
            // The column trying to project does not exist
            fprintf(stderr, "%s\n", "esql: 632");
            return CHIDB_EINVALIDSQL;
        }

        // fprintf(stderr, "Column position is %d\n", col_pos);
        // Create and add the op for putting column contents into registers
        if(col_pos == 0)
            new_op = chidb_make_op(Op_Key, col_c_reg, col_reg, 0, NULL);
        else
            new_op = chidb_make_op(Op_Column, col_c_reg, col_pos, col_reg, NULL);
        list_append(&ops, new_op); // Actually append the op

        // Update col_reg
        col_reg++;
    }
    list_iterator_stop(&snames);

    // Update result row insn offset
    rr_off = col_off + list_size(&snames) + (3*(num_common_cols));

    // Add the result row op.
    new_op = chidb_make_op(Op_ResultRow, first_col_reg, list_size(&snames), 0, NULL);
    list_append(&ops, new_op);

    // *** Add the next op(s) depending on if natural join or not ***
    if(sra_table2 == NULL)
    {
        jump_addr = rewind_off + 1; // Jump to below rewind
        list_append(&ops, chidb_make_op(Op_Next, c1_reg, jump_addr, 0, NULL));
    }
    else
    {
        // Inner next jumps to area right below last rewind when cursor 2 goes to next
        jump_addr = rewind_off + 1;
        list_append(&ops, chidb_make_op(Op_Next, c2_reg, jump_addr, 0, NULL));

        // Outer next jumps to the last rewind (cursor 2 rewind) when cursor 1 goes to next
        jump_addr = rewind_off;
        list_append(&ops, chidb_make_op(Op_Next, c1_reg, jump_addr, 0, NULL));
    }

    // *** Update all of the ops that need an accurate close position ***
    // Or any offset positions that got updated later...
    // Ex: new_op = chidb_make_op(Op_Ne, comp_val_reg, comp_jump_addr, comp_col_reg, NULL);
    // Update at comp_off for the jump to next
    // Update at rewinds for the jump to close
    chidb_dbm_op_t *to_update;

    if(sra_select != NULL)
    {
        jump_addr = rr_off + 1; // This is the current location of next (no nj)
        to_update = (chidb_dbm_op_t *)list_get_at(&ops, comp_off);
        to_update->p2 = jump_addr;
    }

    if(sra_table2 != NULL)
    {
        int i;
        jump_addr = rr_off + 1; // This is location of inner next
        for(i = comp_nj_off - 1; i < comp_nj_off + (3*num_common_cols); i+=3)
        {
            to_update = (chidb_dbm_op_t *)list_get_at(&ops, i);
            to_update->p2 = jump_addr;
        }
    }

    jump_addr = list_size(&ops); // This is the insn addr for close
    to_update = (chidb_dbm_op_t *)list_get_at(&ops, rewind_off);
    to_update->p2 = jump_addr;
    if(sra_table2 != NULL)
    {
        to_update = (chidb_dbm_op_t *)list_get_at(&ops, rewind_off-1);
        to_update->p2 = jump_addr;
    }

    // *** Add the close and halt ops ***
    list_append(&ops, chidb_make_op(Op_Close, c1_reg, 0, 0, NULL));
    if(sra_table2 != NULL)
        list_append(&ops, chidb_make_op(Op_Close, c2_reg, 0, 0, NULL));

    list_append(&ops, chidb_make_op(Op_Halt, 0, 0, 0, NULL));

    // ======================== END CODEGEN SECTION ===========================

    // ------------------convert instructions to stmt struct------------------
    int j;
    for(j = 0; j < list_size(&ops); j++)
    {
        chidb_dbm_op_t *next = (chidb_dbm_op_t *)list_get_at(&ops, j);
        chidb_stmt_set_op(stmt, next, j);
        
        // should be able to free the op now...
        free(next);
    }
    // ------------------convert column names from list to array--------------

    char **cols = malloc(sizeof(char *) * list_size(&snames));
    for(j=0; j < list_size(&snames); j++)
    {
        cols[j] = strdup(list_get_at(&snames, j)); 
    }

    // -------------------- fill in rest of stmt struct ----------------------
    stmt->nRR = list_size(&snames);
    stmt->startRR = first_col_reg;
    stmt->nCols = list_size(&snames);
    stmt->cols = cols;
    // --------------------convenience list destruction-----------------------

    list_destroy(&tnames); 
    list_destroy(&cnames1);
    list_destroy(&cnames2);
    list_destroy(&snames);
    list_destroy(&ops);

    return CHIDB_OK;
}

int chidb_stmt_select_star_expand(SRA_Project_t *sra_project, list_t names, list_t names2)
{
    Expression_t *next_expr = sra_project->expr_list;
    ColumnReference_t *next_ref = next_expr->expr.term.ref;
    char *next_name;
    char *table_name = next_ref->tableName;
    int first_done = 0; // Pseudo bool to take care of first case where malloc is not necessary. just overwrite

    Expression_t *expr_to_add;

    list_iterator_start(&names);
    while(list_iterator_hasnext(&names))
    {
        next_name = (char *)list_iterator_next(&names);
        // fprintf(stderr, "next name is: %s\n", next_name);
        if(!first_done)
        {
            // Free the original star
            free(next_ref->columnName);

            // Overwrite the original column ref 
            next_ref->columnName = strdup(next_name);

            // Ignoring aliasing

            // Now we're done with the first one
            first_done = 1;
        }
        else
        {            
            // Populate all fields of new_term to add to new expression
            ExprTerm new_term;
            new_term.t = TERM_COLREF;
            new_term.ref = malloc(sizeof(ColumnReference_t));

            // Populate all fields of new expression
            expr_to_add = malloc(sizeof(Expression_t));            
            expr_to_add->t = EXPR_TERM;
            expr_to_add->expr.term = new_term;
            expr_to_add->alias = NULL;
            expr_to_add->next = NULL;


            // Append new expression to end
            next_expr->next = expr_to_add;
            
            // Advance to the next expression
            next_expr = expr_to_add;
            next_ref = next_expr->expr.term.ref;


            // Write in the table and column names. alias set to null
            next_ref->tableName = table_name==NULL ? NULL : strdup(table_name);
            next_ref->columnName = strdup(next_name);
            next_ref->columnAlias = NULL;
        }
    }
    list_iterator_stop(&names);

    if(list_size(&names2))
    {
        list_iterator_start(&names2);
        while(list_iterator_hasnext(&names2))
        {
            next_name = (char *)list_iterator_next(&names2);
            // fprintf(stderr, "next name is: %s\n", next_name);
            // Only add column to star expansion if exclusive to table2
            if(chidb_column_position(&names, next_name) < 0)
            {            
                // Populate all fields of new_term to add to new expression
                ExprTerm new_term;
                new_term.t = TERM_COLREF;
                new_term.ref = malloc(sizeof(ColumnReference_t));

                // Populate all fields of new expression
                expr_to_add = malloc(sizeof(Expression_t));            
                expr_to_add->t = EXPR_TERM;
                expr_to_add->expr.term = new_term;
                expr_to_add->alias = NULL;
                expr_to_add->next = NULL;


                // Append new expression to end
                next_expr->next = expr_to_add;
                
                // Advance to the next expression
                next_expr = expr_to_add;
                next_ref = next_expr->expr.term.ref;


                // Write in the table and column names. alias set to null
                next_ref->tableName = table_name==NULL ? NULL : strdup(table_name);
                next_ref->columnName = strdup(next_name);
                next_ref->columnAlias = NULL;
            }
        }
        list_iterator_stop(&names2);
    }

    return CHIDB_OK;
}


//Main function that calls all the helpers
int chidb_stmt_codegen(chidb_stmt *stmt, chisql_statement_t *sql_stmt)
{
    int ret = 0;
    list_t tables;

    if (chidb_stmt_check(stmt, sql_stmt, tables) != CHIDB_OK)
        return CHIDB_EINVALIDSQL;

    //Refresh the in-memory schema table if necessary
    if(stmt->db->need_refresh == 1) {
        list_iterator_start(&(stmt->db->schemas));
        while(list_iterator_hasnext(&(stmt->db->schemas)))
        {
            chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&(stmt->db->schemas)));
            free(next);
        }
        list_iterator_stop(&(stmt->db->schemas));
        list_destroy(&(stmt->db->schemas));

        list_init(&(stmt->db->schemas));
        load_schema(stmt->db,1);
        stmt->db->need_refresh = 0;

        // fprintf(stderr, "%s\n", "Schema has been refreshed");
        //print_schema_list(stmt->db->schemas);
    }

    switch(sql_stmt->type)
    {
        case STMT_CREATE: 
            ret =  chidb_stmt_create(stmt, sql_stmt);
            break;
        case STMT_SELECT: 
            ret =  chidb_stmt_select(stmt, sql_stmt);
            break;
        case STMT_INSERT: 
            ret =  chidb_stmt_insert(stmt, sql_stmt);
            break;
    }

    return ret;
}
