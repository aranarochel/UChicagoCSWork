/*
 *  chidb - a didactic relational database management system
 *
 *  Miscellaneous functions
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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "chidbInt.h"
#include "util.h"
#include "record.h"
#include "../simclist/simclist.h"


// Given a table name, determine whether such a table exists.
int chidb_table_exists(list_t s, char *table)
{
    list_iterator_start(&s);
    while(list_iterator_hasnext(&s))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&s));
        // fprintf(stderr, "checking table name: %s\n", next->name);
        if(!strcmp(next->name, table))
        {
            return CHIDB_OK;
        }
    }

    list_iterator_stop(&s);

    return CHIDB_EINVALIDSQL;
}

// Given a table name, obtain its root page.
int chidb_get_root(list_t s, char *table)
{
    list_iterator_start(&s);
    while(list_iterator_hasnext(&s))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&s));
        if(!strcmp(next->name, table))
        {
            list_iterator_stop(&s);
            return next->rpage;
        }
    }

    list_iterator_stop(&s);

    return CHIDB_EINVALIDSQL;
}

// Given a table name and a column name, determine whether such a column exists in the table.
int chidb_column_exists(list_t s, char *table, char *column)
{
    list_iterator_start(&s);
    while(list_iterator_hasnext(&s))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&s));
        if(!strcmp(next->name, table))
        {
            Column_t *next_column = next->stmt->stmt.create->table->columns;
            while(next_column != NULL)
            {
                if(!strcmp(next_column->name, column))
                {
                    list_iterator_stop(&s);
                    return CHIDB_OK;
                }
                else
                    next_column = next_column->next;
            }
            list_iterator_stop(&s);
            return CHIDB_EINVALIDSQL;
        }
    }

    list_iterator_stop(&s);

    return CHIDB_EINVALIDSQL;
}

// Given a table name and a column name, obtain the type of the column.
int chidb_column_get_type(list_t s, char *table, char *column)
{
    list_iterator_start(&s);
    while(list_iterator_hasnext(&s))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&s));
        if(!strcmp(next->name, table))
        {
            Column_t *next_column = next->stmt->stmt.create->table->columns;
            while(next_column != NULL)
            {
                if(!strcmp(next_column->name, column))
                {
                    list_iterator_stop(&s);
                    return next_column->type;
                }
                else
                    next_column = next_column->next;
            }
            list_iterator_stop(&s);
            return CHIDB_EINVALIDSQL;
            
        }
    }

    list_iterator_stop(&s);

    return CHIDB_EINVALIDSQL;
}

// S is the schema table
int chidb_column_names(list_t s, char *table, list_t *names)
{
    list_iterator_start(&s);
    while(list_iterator_hasnext(&s))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&s));
        if(!strcmp(next->name, table))
        {
            Column_t *next_column = next->stmt->stmt.create->table->columns;
            if(next_column ==NULL) //if it starts null then there are no columns
            {
                return CHIDB_EINVALIDSQL;
            }

            while(next_column != NULL)
            {
                // fprintf(stderr, "Adding %s as column name\n", next_column->name);
                list_append(names, next_column->name);

                next_column = next_column->next;
            }
            return CHIDB_OK;
        }
    }

    list_iterator_stop(&s);

    return CHIDB_EINVALIDSQL;
}

// Returns number of columns for a specified schema table
int chidb_columns_total(list_t schemas, char *table)
{
    int i = 0;  // Return value of number of columns

    list_iterator_start(&schemas);
    while(list_iterator_hasnext(&schemas))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&schemas));
        if(!strcmp(next->name,table))
        {
            Column_t *column = next->stmt->stmt.create->table->columns;
            while(column != NULL)
            {
                i++;
                column = column->next;
            }
        }
    }
    list_iterator_stop(&schemas);

    return i;
}

// print the schema list found in db->schemas
void print_schema_list(list_t schemas)
{
    int i = 0;
    fprintf(stderr, "***** PRINTING SCHEMAS LIST *****\n\n");
    list_iterator_start(&schemas);
    while(list_iterator_hasnext(&schemas))
    {
        chidb_sql_schema_t *next = (chidb_sql_schema_t *)(list_iterator_next(&schemas));
        fprintf(stderr, "* Schema No. %d *\n",i);
        fprintf(stderr, "  Type: %s\n",next->type);
        fprintf(stderr, "  Name: %s\n",next->name);
        fprintf(stderr, "  Assoc: %s\n",next->assoc);
        fprintf(stderr, "  Root Page: %d\n",next->rpage);
        chisql_stmt_print(next->stmt);
        i++;
    }
    list_iterator_stop(&schemas);
}

// Given a SQL statement, extract the names of all tables involved
int chidb_get_tables(list_t tables, chisql_statement_t *sql_statement)
{
    switch (sql_statement->type)
    {        
        case STMT_SELECT:
            if (chidb_get_sra_tables(tables, sql_statement->stmt.select) != CHIDB_OK)
                return CHIDB_EINVALIDSQL;
            break;
   
        case STMT_INSERT:
            list_append(&tables, sql_statement->stmt.insert->table_name);
            break;

        case STMT_DELETE: 
            list_append(&tables, sql_statement->stmt.delete->table_name);
           break;
        case STMT_CREATE:
            break;
    }

    return CHIDB_OK;
}

// recursively descend the the AST until you get to a table and grab the name 
int chidb_get_sra_tables(list_t tables, SRA_t *s)
{
    int type = s->t;

    if (type == SRA_TABLE)
    {
        list_append(&tables, s->table.ref->table_name);
    }
    else if (type == SRA_PROJECT)
    {
        if (chidb_get_sra_tables(tables, s->project.sra) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    else if (type == SRA_SELECT)
    {
        if (chidb_get_sra_tables(tables, s->select.sra) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    else if (SRA_NATURAL_JOIN <= type && type <= SRA_RIGHT_OUTER_JOIN)
    {
        if (chidb_get_sra_tables(tables, s->join.sra1) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
        if (chidb_get_sra_tables(tables, s->join.sra2) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    else if (SRA_UNION <= type && type <= SRA_INTERSECT)
    {
        if (chidb_get_sra_tables(tables, s->binary.sra1) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
        if (chidb_get_sra_tables(tables, s->binary.sra2) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    else
    {
        return CHIDB_EPARSE;
    }

    return CHIDB_OK;
}

/* Given an initialized list to store column names (char*'s), and the expression
 * list found in project SRA struct, appends names of all specified selection columns
 * found in SELECT query to list
 */
int chidb_get_select_columns(list_t column_names, Expression_t *exp_list)
{
    while(exp_list != NULL)
    {
        // If we try select something that's not a term aka column name
        if(exp_list->t != EXPR_TERM) {
            return CHIDB_EINVALIDSQL;
        }
        else {
            // If we try to select something that isn't a column reference
            // a.k.a SELECT 42 FROM courses
            if(exp_list->expr.term.t != TERM_COLREF) {
                return CHIDB_EINVALIDSQL;
            }
            else {
                // Add the column name to list
                // Note this does not add any possible alias names
                list_append(&column_names,exp_list->expr.term.ref->columnName);
            }
        }
        exp_list = exp_list->next;
    }
    return CHIDB_OK;
}

/* Given an initialized list to store the names of the values in where clauses,
 * and the cond struct found in the SRA Select struct, return list of tables
 * associated with each value. Ex. t.a = 1, would mean we would store table name t.
 * E.x 2 < 1 means we would store nothing. 
 */
int chidb_get_where_tables(list_t tables, Condition_t *cond)
{
    // int ret = 0;
    Condition_print(cond);

    if(cond->t == RA_COND_AND)
    {
        chidb_get_where_tables(tables, cond->cond.binary.cond1);
        chidb_get_where_tables(tables, cond->cond.binary.cond2);
    }
    else
    {
        Expression_t *e1 = cond->cond.comp.expr1;
        Expression_t *e2 = cond->cond.comp.expr2;

        if(e1->t == EXPR_TERM)
        {
            if(e1->expr.term.t == TERM_COLREF)
            {
                list_append(&tables, e1->expr.term.ref->tableName);
            }
        }

        if(e2->t == EXPR_TERM)
        {
            if(e2->expr.term.t == TERM_COLREF)
            {
                list_append(&tables, e2->expr.term.ref->tableName);
            }
        }

    }

    return list_size(&tables);
}

int chidb_get_where_columns(list_t column_names, Condition_t *cond)
{
    // int ret = 0;

    if (cond->t == RA_COND_AND || cond->t == RA_COND_OR)
    {
        chidb_get_where_columns(column_names, cond->cond.binary.cond1);
        chidb_get_where_columns(column_names, cond->cond.binary.cond2);
    }
    else if (cond->t <= RA_COND_GEQ)
    {
        Expression_t *e1 = cond->cond.comp.expr1;
        Expression_t *e2 = cond->cond.comp.expr2;

        if(e1->t == EXPR_TERM)
        {
            if(e1->expr.term.t == TERM_COLREF)
            {
                list_append(&column_names, e1->expr.term.ref->columnName);
            }
        }

        if(e2->t == EXPR_TERM)
        {
            if(e2->expr.term.t == TERM_COLREF)
            {
                list_append(&column_names, e2->expr.term.ref->columnName);
            }
        }
    }

    return list_size(&column_names);
}

// int chidb_get_where_conds(list_t conds, SRA_t *s)
// {
//     int type = s->t;

//     if (type == SRA_TABLE)
//     {
//         return CHIDB_OK;
//     }
//     else if (type == SRA_PROJECT)
//     {
//         if (chidb_get_where_conds(conds, s->project.sra) != CHIDB_OK)
//             return CHIDB_EINVALIDSQL;
//     }
//     else if (type == SRA_SELECT)
//     {
//         if (s->select.cond->t <= RA_COND_GEQ)
//         {
//             list_append(&conds, s->select.cond);
//             Condition_print(s->select.cond);
//             // list_append(&conds, s->select.cond->cond.comp.expr1);
//             // list_append(&conds, s->select.cond->cond.comp.expr2);
//         }
//         else if (s->select.cond->t == RA_COND_AND)
//         {
//             // Condition_print(s->select.cond->cond.binary.cond1);
//             list_append(&conds, &s->select.cond->cond.binary.cond1);
//             list_append(&conds, &s->select.cond->cond.binary.cond2);
//         }
//     }
//     else if (SRA_NATURAL_JOIN <= type && type <= SRA_RIGHT_OUTER_JOIN)
//     {
//         if (chidb_get_where_conds(conds, s->join.sra1) != CHIDB_OK)
//             return CHIDB_EINVALIDSQL;
//         if (chidb_get_where_conds(conds, s->join.sra2) != CHIDB_OK)
//             return CHIDB_EINVALIDSQL;
//     }
//     else if (SRA_UNION <= type && type <= SRA_INTERSECT)
//     {
//         if (chidb_get_where_conds(conds, s->binary.sra1) != CHIDB_OK)
//             return CHIDB_EINVALIDSQL;
//         if (chidb_get_where_conds(conds, s->binary.sra2) != CHIDB_OK)
//             return CHIDB_EINVALIDSQL;
//     }
//     else
//     {
//         return CHIDB_EPARSE;
//     }

//     return CHIDB_OK;
// }

int chidb_columns_in_same_table(list_t s, list_t tables, list_t found_tables, char *col1, char *col2)
{
    list_iterator_start(&tables);
    while(list_iterator_hasnext(&tables))
    {
        char *t = (char *) list_iterator_next(&tables);
        if (chidb_column_exists(s, t, col1) && chidb_column_exists(s, t, col2))
        {
            list_append(&found_tables, t);
        }
    }    
    list_iterator_stop(&tables);

    return list_size(&found_tables);
}

int nameSeeker(const void *entry, const void *name)
{
    if (!strncmp((char *) entry, name, strlen(name)))
        return 1;
    else
        return 0;
}

/* given an in-order list of column names according to the table, 
 * return the column number (0 indexed)
 *
 * returns -1 if the column name was not found (errrror)
 */
int chidb_column_position(list_t *names, char *col_name)
{
    int i = 0;
    char *next_name;

    list_iterator_start(names);
    while(list_iterator_hasnext(names))
    {
        next_name = (char *)list_iterator_next(names);
        if(!strcmp(next_name, col_name))
            break;
        else
            i++;

    }
    list_iterator_stop(names);

    return (i == list_size(names)) ? (-1) : (i);
}


/*
** Read or write a four-byte big-endian integer value.
* Based on SQLite code
*/
uint32_t get4byte(const uint8_t *p)
{
    return (p[0]<<24) | (p[1]<<16) | (p[2]<<8) | p[3];
}

void put4byte(unsigned char *p, uint32_t v)
{
    p[0] = (uint8_t)(v>>24);
    p[1] = (uint8_t)(v>>16);
    p[2] = (uint8_t)(v>>8);
    p[3] = (uint8_t)v;
}

int getVarint32(const uint8_t *p, uint32_t *v)
{
    *v = 0;

    *v  = ((uint32_t) (p[3] & 0x7F));
    *v |= ((uint32_t) (p[2] & 0x7F)) << 7;
    *v |= ((uint32_t) (p[1] & 0x7F)) << 14;
    *v |= ((uint32_t) (p[0] & 0x7F)) << 21;

    return CHIDB_OK;
}

int putVarint32(uint8_t *p, uint32_t v)
{
    uint32_t aux;

    aux = v & 0x0000007F;
    aux |= ((v & 0x00003F80) << 1) | 0x00008000;
    aux |= ((v & 0x001FC000) << 2) | 0x00800000;
    aux |= ((v & 0x0FE00000) << 3) | 0x80000000;

    p[3] = (uint8_t)(aux);
    p[2] = (uint8_t)(aux>>8);
    p[1] = (uint8_t)(aux>>16);
    p[0] = (uint8_t)(aux>>24);

    return CHIDB_OK;
}


void chidb_BTree_recordPrinter(BTreeNode *btn, BTreeCell *btc)
{
    DBRecord *dbr;

    chidb_DBRecord_unpack(&dbr, btc->fields.tableLeaf.data);

    printf("< %5i >", btc->key);
    chidb_DBRecord_print(dbr);
    printf("\n");

    free(dbr);
}

void chidb_BTree_stringPrinter(BTreeNode *btn, BTreeCell *btc)
{
    printf("%5i -> %10s\n", btc->key, btc->fields.tableLeaf.data);
}

int chidb_astrcat(char **dst, char *src)
{
    *dst = realloc(*dst, strlen(*dst) + strlen(src) + sizeof(char));
    if (!*dst)
        return CHIDB_ENOMEM;
    strcat(*dst, src);
    return CHIDB_OK;
}


int chidb_Btree_print(BTree *bt, npage_t npage, fBTreeCellPrinter printer, bool verbose)
{
    BTreeNode *btn;

    chidb_Btree_getNodeByPage(bt, npage, &btn);

    if (btn->type == PGTYPE_TABLE_LEAF)
    {
        if (verbose)
            printf("Leaf node (page %i)\n", btn->page->npage);
        for(int i = 0; i<btn->n_cells; i++)
        {
            BTreeCell btc;

            chidb_Btree_getCell(btn, i, &btc);
            chidb_BTree_stringPrinter(btn, &btc);
        }
    }
    else if (btn->type == PGTYPE_TABLE_INTERNAL)
    {
        chidb_key_t last_key;

        if(verbose)
            printf("Internal node (page %i)\n", btn->page->npage);
        for(int i = 0; i<btn->n_cells; i++)
        {
            BTreeCell btc;

            chidb_Btree_getCell(btn, i, &btc);

            last_key = btc.key;
            if(verbose)
                printf("Printing Keys <= %i\n", last_key);
            chidb_Btree_print(bt, btc.fields.tableInternal.child_page, printer, verbose);
        }
        if(verbose)
            printf("Printing Keys > %i\n", last_key);
        chidb_Btree_print(bt, btn->right_page, printer, verbose);
    }
    else if (btn->type == PGTYPE_INDEX_LEAF)
    {
        if (verbose)
            printf("Leaf node (page %i)\n", btn->page->npage);
        for(int i = 0; i<btn->n_cells; i++)
        {
            BTreeCell btc;

            chidb_Btree_getCell(btn, i, &btc);
            printf("%10i -> %10i\n", btc.key, btc.fields.indexLeaf.keyPk);
        }
    }
    else if (btn->type == PGTYPE_INDEX_INTERNAL)
    {
        chidb_key_t last_key;

        if(verbose)
            printf("Internal node (page %i)\n", btn->page->npage);
        for(int i = 0; i<btn->n_cells; i++)
        {
            BTreeCell btc;

            chidb_Btree_getCell(btn, i, &btc);
            last_key = btc.key;
            if(verbose)
                printf("Printing Keys < %i\n", last_key);
            chidb_Btree_print(bt, btc.fields.indexInternal.child_page, printer, verbose);
            printf("%10i -> %10i\n", btc.key, btc.fields.indexInternal.keyPk);
        }
        if(verbose)
            printf("Printing Keys > %i\n", last_key);
        chidb_Btree_print(bt, btn->right_page, printer, verbose);
    }

    chidb_Btree_freeMemNode(bt, btn);

    return CHIDB_OK;
}

FILE *copy(const char *from, const char *to)
{
    FILE *fromf, *tof;
    char ch;

    if( (fromf = fopen(from, "rb")) == NULL || (tof = fopen(to, "wb")) == NULL)
        return NULL;

    /* copy the file */
    while(!feof(fromf))
    {
        ch = fgetc(fromf);
        if(ferror(fromf))
            return NULL;
        fputc(ch, tof);
        if(ferror(tof))
            return NULL;
    }

    if(fclose(fromf)==EOF || fclose(tof)==EOF)
        return NULL;

    return tof;
}


int chidb_tokenize(char *str, char ***tokens)
{
    char *s;
    int ntokens = 0;

    s = str;
    if (s==NULL)
        return CHIDB_ENOMEM;

    /* First pass: Add \0 at the end of each token
     * and count the number of tokens */
    while(isspace(*s)) s++;

    while(*s != '\0')
    {
        ntokens++;
        if (*s == '"')
        {
            s++;
            while(*s && *s != '"') s++;
        }
        else
            while(*s && !isspace(*s)) s++;

        if(*s != '\0')
        {
            *s++ = '\0';
            while(*s && isspace(*s)) s++;
        }
    }


    /* Second pass: Create the array of tokens */
    *tokens = malloc(sizeof(char**) * ntokens);

    s = str;
    while(isspace(*s)) s++;
    for(int i=0; i<ntokens; i++)
    {
        if (*s == '"')
        {
            s++;
            (*tokens)[i] = s;
            while(*s && *s != '"') s++;
        }
        else
        {
            (*tokens)[i] = s;
            while(*s && !isspace(*s)) s++;
        }

        s++;
        while(*s && isspace(*s) && (*s != '\0')) s++;
    }

    return ntokens;
}

void chisql_statement_free(chisql_statement_t *sql)
{
    switch (sql->type)
    {
        case STMT_CREATE:
        {
            Create_free(sql->stmt.create);
        } break;

        case STMT_SELECT:
        {
            SRA_free(sql->stmt.select);
        } break;

        case STMT_INSERT:
        {
            Insert_free(sql->stmt.insert);
        } break;

        case STMT_DELETE:
        {
            Delete_free(sql->stmt.delete);
        } break;
    }

    free(sql->text);
    free(sql);
}
