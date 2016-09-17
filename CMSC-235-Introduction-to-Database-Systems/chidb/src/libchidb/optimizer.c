/*
 *  chidb - a didactic relational database management system
 *
 *  Query Optimizer
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
#include "dbm-types.h"

#define CHIDB_DONT_OPT (808)

int chidb_sql_optimize_check(chisql_statement_t *sql_stmt);
int chidb_sra_optimize_check(SRA_t *sra_select);
int chidb_sigma_push_one_cond(chidb_stmt *stmt, SRA_t *sra_select, Condition_t *cond);
int chidb_sigma_push(chidb_stmt *stmt, SRA_t *sra_select);
int chidb_sigma_cond_simplify(Condition_t *cond, Condition_t **out_cond);
int chidb_sigma_evaluate(Condition_t *cond);
int chidb_sigma_push_SRA(chidb_stmt *stmt, SRA_t *sra, Condition_t *cond);
int chidb_sigma_where_loop(chidb_stmt *stmt, SRA_t *s, Condition_t *c);
// int chidb_get_where_tables(Condition_t *cond, char *first, char *second);

int chidb_get_where_tables(list_t tables, Condition_t *cond);
int chidb_get_where_columns(list_t column_names, Condition_t *cond);
int chidb_get_where_conds(list_t conds, SRA_t *s);
int chidb_get_sra_tables(list_t tables, SRA_t *s);

void Condition_print(Condition_t *cond);

int chidb_stmt_optimize(chidb_stmt *stmt, chisql_statement_t *sql_stmt, chisql_statement_t **sql_stmt_opt)
{
    // do initial check to see if we need to optimize
    int opt_check = chidb_sql_optimize_check(sql_stmt);

    // if check returned CHIDB_DONT_OPT, just return copy of provided statement
    if (opt_check == CHIDB_EINVALIDSQL)
    {
    	fprintf(stderr, "%s\n\n", "ERROR: SQL statement is not well-formed");
    	return CHIDB_EINVALIDSQL;
    }
 	if(opt_check != CHIDB_OK)
 	{
 		// check verified that no optimization needs to be done
 		*sql_stmt_opt = malloc(sizeof(chisql_statement_t));
    	memcpy(*sql_stmt_opt, sql_stmt, sizeof(chisql_statement_t));
    	fprintf(stderr, "%s\n\n", "No sigma optimization to be completed.");
 	}
 	else
 	{
 		// jaime: need to do actual sigma pushing beyond here
 		*sql_stmt_opt = malloc(sizeof(chisql_statement_t));

 		// do sigma pushing
 		int ret = chidb_sigma_push(stmt, sql_stmt->stmt.select);

    	memcpy(*sql_stmt_opt, sql_stmt, sizeof(chisql_statement_t));
 	}

    return CHIDB_OK;
}

/* This does a rudimentary check on sql statements to see if it can be optimized.
 * Handles cases like:
 		SELECT * FROM t
 		SELECT * FROM t |><| u
 		SELECT * FROM t WHERE [val-or-col] [oper] [val-or-col]
   which should be ignored by the optimizer
 *
 */
int chidb_sql_optimize_check(chisql_statement_t *sql_stmt)
{
	// check if we're even dealing with a select statement
	if(sql_stmt->type != STMT_SELECT)
	{
		return CHIDB_DONT_OPT;
	}
	
	int sra_ret = chidb_sra_optimize_check(sql_stmt->stmt.select);

	return sra_ret;

}

/* Used in chidb_sql_optimize_check. This works directly with the select SRA struct
 * and is needed because of the fact that we might have selects inside unions
 */
int chidb_sra_optimize_check(SRA_t *sra_select)
{
	int opt_ret = 0;

	if(sra_select->t == SRA_UNION)
	{
		int ret1 = chidb_sra_optimize_check(sra_select->binary.sra1);
		int ret2 = chidb_sra_optimize_check(sra_select->binary.sra2);

		// if either of the two sub queries doesn't return CHIDB_OK, signaling that opt can initiate,
		// then return CHIDB_DONT_OPT.
		if(ret1 == CHIDB_OK || ret2 == CHIDB_OK)
		{
			return CHIDB_OK;
		}
		else {
			return CHIDB_DONT_OPT;
		}
	}

	// get the SRA struct that comes after the PROJECT struct
	SRA_t *select = sra_select->project.sra;

	// SELECT * FROM t
	if(select->t == SRA_TABLE)
	{
		opt_ret = CHIDB_DONT_OPT;
	}
	// SELECT * FROM t |><| u
	else if(select->t == SRA_NATURAL_JOIN)
	{
		opt_ret = CHIDB_DONT_OPT;
	}
	// SELECT * FROM t WHERE ...
	else if(select->t == SRA_SELECT)
	{
		// Simplify the condition if it needs to be
		Condition_t *c = NULL;

		int ret = chidb_sigma_cond_simplify(select->select.cond, &c);

		// we don't actually need the select struct...
		if (ret == CHIDB_TRUE)
		{
			SRA_t *new = SRAProject(select->select.sra, sra_select->project.expr_list);
			ProjectOption_t *p1 = malloc(sizeof(ProjectOption_t));
			ProjectOption_t *p2 = malloc(sizeof(ProjectOption_t));
			p1 = OrderBy_make(sra_select->project.order_by, sra_select->project.asc_desc);
			p2 = GroupBy_make(sra_select->project.group_by);
			ProjectOption_t *option = ProjectOption_combine(p1, p2);
			SRA_applyOption(new, option);

			memcpy(&sra_select->project, &new->project, sizeof(SRA_Project_t));
			return CHIDB_DONT_OPT;
		}
		// Problem in the WHERE clause
		else if (ret == CHIDB_EINVALIDSQL)
			return CHIDB_EINVALIDSQL;

		// Updated (maybe) condition struct
		else if (ret == CHIDB_OK)
		{
			// fprintf(stderr, "%s\n", "ret was ok");
			// Condition_print(select->select.cond);
			// Condition_print(c);
			memcpy(select->select.cond, c, sizeof(Condition_t));
			// select->select.cond = c;
			// Condition_free(c);
		}

		// Get on with the checking...
		SRA_t *select_where = select->select.sra;
		if(select_where->t == SRA_TABLE)
		{
			opt_ret = CHIDB_DONT_OPT;
		}
		// SELECT * FROM t |><| u WHERE ...
		else if(select_where->t == SRA_NATURAL_JOIN)
		{
			opt_ret = CHIDB_OK;
		}
	}

	return opt_ret;
}

// I'm thinking this might be the "main" function which calls sub push routines
int chidb_sigma_push(chidb_stmt *stmt, SRA_t *sra_select)
{
	// Do error checking on table names and column/value types!

	// if we're dealing with a union
	if(sra_select->t == SRA_UNION)
	{
		fprintf(stderr, "FIRST SQL IN UNION\n");
		chidb_sigma_push(stmt, sra_select->binary.sra1);
		fprintf(stderr, "SECOND SQL IN UNION\n");
		chidb_sigma_push(stmt, sra_select->binary.sra2);
	}
	
	SRA_t *select = sra_select->project.sra;

	if (select->select.cond == NULL)
		return CHIDB_OK;

	// check if we're dealing with mutiple AND clauses or not
	if(select->select.cond->t == RA_COND_AND || select->select.cond->t == RA_COND_OR)
	{
		fprintf(stderr, "%s\n", "PUSHING AND/OR");

		// do pushing stuff dealing with mutiple and clauses
		// list_t conditions;
		// list_init(&conditions);
		// int list_sz = chidb_get_where_conds(conditions, select);

		// fprintf(stderr, "%s%i\n", "list size was: ", list_sz);

		// list_iterator_start(&conditions);
		// while (list_iterator_hasnext(&conditions))
		// {
		// 	Condition_t *c = (Condition_t *) list_iterator_next(&conditions);
		// 	Condition_print(c);
		// 	fprintf(stderr, "%s\n", "parsing a condition");
		// 	// chidb_sigma_push_SRA(stmt, select, c);
		// 	chidb_sigma_push_one_cond(stmt, select, c);
		// }
		// list_iterator_stop(&conditions);

		chidb_sigma_where_loop(stmt, sra_select, select->select.cond);

		return CHIDB_OK;
	}
	else
	{
		fprintf(stderr, "ABOUT TO PUSH WITH ONE COND\n");
		// do pushing with just one and clause
		// chidb_sigma_push_SRA(stmt, select, select->select.cond);
		// fprintf(stderr, "%s%i\n", "type of condition: ", select->select.cond->t);
		chidb_sigma_push_one_cond(stmt, sra_select, select->select.cond);

	}

	return CHIDB_OK;
}

int chidb_sigma_where_loop(chidb_stmt *stmt, SRA_t *s, Condition_t *c)
{
	if (c->cond.binary.cond1->t <= RA_COND_GEQ)
	{
		chidb_sigma_push_one_cond(stmt, s, c->cond.binary.cond1);
	}
	else if (c->cond.binary.cond1->t == RA_COND_AND || c->cond.binary.cond1->t == RA_COND_OR)
	{
		chidb_sigma_where_loop(stmt, s, c->cond.binary.cond1->cond.binary.cond1);
		chidb_sigma_where_loop(stmt, s, c->cond.binary.cond1->cond.binary.cond1);
	}
	
	if (c->cond.binary.cond2->t <= RA_COND_GEQ)
	{
		chidb_sigma_push_one_cond(stmt, s, c->cond.binary.cond2);	
	}
	else if (c->cond.binary.cond2->t == RA_COND_AND || c->cond.binary.cond2->t == RA_COND_OR)
	{
		chidb_sigma_where_loop(stmt, s, c->cond.binary.cond2->cond.binary.cond1);
		chidb_sigma_where_loop(stmt, s, c->cond.binary.cond2->cond.binary.cond1);
	}

	return CHIDB_OK;
}


int chidb_get_where_conds(list_t conds, SRA_t *s)
{
    int type = s->t;

    if (type == SRA_TABLE)
    {
        return CHIDB_OK;
    }
    else if (type == SRA_PROJECT)
    {
        if (chidb_get_where_conds(conds, s->project.sra) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
    }
    else if (type == SRA_SELECT)
    {
        if (s->select.cond->t <= RA_COND_GEQ)
        {
            list_append(&conds, s->select.cond);
            Condition_print(s->select.cond);
            // list_append(&conds, s->select.cond->cond.comp.expr1);
            // list_append(&conds, s->select.cond->cond.comp.expr2);
        }
        else if (s->select.cond->t == RA_COND_AND)
        {
            // Condition_print(s->select.cond->cond.binary.cond1);
            list_append(&conds, &s->select.cond->cond.binary.cond1);
            list_append(&conds, &s->select.cond->cond.binary.cond2);
        }
    }
    else if (SRA_NATURAL_JOIN <= type && type <= SRA_RIGHT_OUTER_JOIN)
    {
    	list_t conds1;
    	list_init(&conds1);
    	list_t conds2;
    	list_init(&conds2);
        if (chidb_get_where_conds(conds1, s->join.sra1) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
        if (chidb_get_where_conds(conds2, s->join.sra2) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
        list_concat(&conds1, &conds2, &conds);
    }
    else if (SRA_UNION <= type && type <= SRA_INTERSECT)
    {
    	list_t conds1;
    	list_init(&conds1);
    	list_t conds2;
    	list_init(&conds2);
        if (chidb_get_where_conds(conds, s->binary.sra1) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
        if (chidb_get_where_conds(conds, s->binary.sra2) != CHIDB_OK)
            return CHIDB_EINVALIDSQL;
        list_concat(&conds1, &conds2, &conds);
    }
    else
    {
        return CHIDB_EPARSE;
    }

    return list_size(&conds);
}

/* This function pushes sigmas where there are no AND clauses.
 * The idea is that we differentiate between that case and this one separately but,
 * we can just join 'em later. This function assumes that error checking is done
 * by the function that calls this.
 */
int chidb_sigma_push_one_cond(chidb_stmt *stmt, SRA_t *sra_select, Condition_t *cond)
{
	// get the SRA struct that comes after the PROJECT struct.
	// this struct will get modified in that we replace the nat join sra
	// it has with the table sra that the where clause references
	SRA_t *select = sra_select->project.sra;

	SRA_t *join = NULL;
	SRA_t *bin  = NULL;

	// get natural join struct that comes after the select struct
	if (select->select.sra->t >= SRA_NATURAL_JOIN || select->select.sra->t <= SRA_RIGHT_OUTER_JOIN)
		join = select->select.sra;
	else if (select->select.sra->t >= SRA_UNION)
		bin  = select->select.sra;

	// get list of table names from WHERE clause
	list_t table_names;
	list_init(&table_names);
	char *table1 = malloc(sizeof(char)*15);
	char *table2 = malloc(sizeof(char)*15);
	// int list_sz = chidb_get_where_tables(select->select.cond, table1, table2);
	int list_sz = chidb_get_where_tables(table_names, select->select.cond);
	// fprintf(stderr, "%s%i\n", "list size was: ", list_sz);	


	// if the tables list is empty, that indicates that the WHERE clause referenced no columns by their table,
	// --and thus we can just push the sigma down both branches-- not true.
	if(list_sz == 0)
	{
		fprintf(stderr, "%s\n", "no tables mentioned");
		// do sigma pushing to both branches
		// E.x: WHERE 1 < 2

		// make copies of cond to make two new select SRA structs
		Condition_t *c1 = malloc(sizeof(Condition_t));
		Condition_t *c2 = malloc(sizeof(Condition_t));
		memcpy(c1, select->select.cond, sizeof(Condition_t));
		memcpy(c2, select->select.cond, sizeof(Condition_t));

		// make copies of the two SRA table structs too
		SRA_t *t1 = malloc(sizeof(SRA_t));
		SRA_t *t2 = malloc(sizeof(SRA_t));
		memcpy(t1, join->binary.sra1, sizeof(SRA_t));
		memcpy(t2, join->binary.sra2, sizeof(SRA_t));


		SRA_t *s1 = malloc(sizeof(SRA_t));
		SRA_t *s2 = malloc(sizeof(SRA_t));
		s1 = SRASelect(t1, c1);
		s2 = SRASelect(t2, c2);

		// make new NATURAL JOIN STRUCT
		SRA_t *natj = malloc(sizeof(SRA_t));
		natj = SRANaturalJoin(s1, s2);

		// change some pointer stuff
		sra_select->project.sra = natj;
	}
	// if there's only one column ref, then just push sigma down appropriate branch
	else if(list_sz == 1)
	{
		// do sigma pushing
		// E.x: WHERE t.a = 2

		// get table name referenced in WHERE clause
		list_iterator_start(&table_names);
		char *table = (char*)(list_iterator_next(&table_names));
		list_iterator_stop(&table_names);

		// if the table referenced is the first table of the NAT JOIN
		// push down that branch
		if(!strcmp(join->binary.sra1->table.ref->table_name, table))
		{
			// make copy of cond struct
			Condition_t *c1 = malloc(sizeof(Condition_t));
			memcpy(c1, select->select.cond, sizeof(Condition_t));

			// make copies of the two SRA table structs too
			SRA_t *t1 = malloc(sizeof(SRA_t));
			SRA_t *t2 = malloc(sizeof(SRA_t));
			memcpy(t1, join->binary.sra1, sizeof(SRA_t));
			memcpy(t2, join->binary.sra2, sizeof(SRA_t));

			// make new NATURAL JOIN STRUCT
			SRA_t *s1 = malloc(sizeof(SRA_t));
			s1 = SRASelect(t1, c1);

			SRA_t *natj = malloc(sizeof(SRA_t));
			natj = SRANaturalJoin(s1,t2);

			// change pointers
			sra_select->project.sra = natj;
		}
		else if(!strcmp(join->binary.sra2->table.ref->table_name, table))
		{
			// make copy of cond struct
			Condition_t *c1 = malloc(sizeof(Condition_t));
			memcpy(c1, select->select.cond, sizeof(Condition_t));

			// make copies of the two SRA table structs too
			SRA_t *t1 = malloc(sizeof(SRA_t));
			SRA_t *t2 = malloc(sizeof(SRA_t));
			memcpy(t1, join->binary.sra1, sizeof(SRA_t));
			memcpy(t2, join->binary.sra2, sizeof(SRA_t));

			// make new NATURAL JOIN STRUCT
			SRA_t *s2 = malloc(sizeof(SRA_t));
			s2 = SRASelect(t2, c1);

			SRA_t *natj = malloc(sizeof(SRA_t));
			natj = SRANaturalJoin(t1,s2);

			// change pointers
			sra_select->project.sra = natj;
		}
	}
	// if the list is greater than 1, a.k.a 2, then we check whether it refers to the same table
	else
	{
		// E.x: WHERE t.a < t.b || t.a > u.c

		// get table names referenced in WHERE clause
		list_iterator_start(&table_names);
		char *table1 = (char *) list_iterator_next(&table_names);
		char* table2 = (char *) list_iterator_next(&table_names);
		list_iterator_stop(&table_names);

		// if the two tables referenced are the same, push down appropriate branch
		if(!strcmp(table1, table2))
		{
			// if tables referenced match first table in nat join
			if(!strcmp(join->binary.sra1->table.ref->table_name, table1))
			{
				// make copy of cond struct
				Condition_t *c1 = malloc(sizeof(Condition_t));
				memcpy(c1, select->select.cond, sizeof(Condition_t));

				// make copies of the two SRA table structs too
				SRA_t *t1 = malloc(sizeof(SRA_t));
				SRA_t *t2 = malloc(sizeof(SRA_t));
				memcpy(t1, join->binary.sra1, sizeof(SRA_t));
				memcpy(t2, join->binary.sra2, sizeof(SRA_t));

				// make new NATURAL JOIN STRUCT
				SRA_t *s1 = malloc(sizeof(SRA_t));
				s1 = SRASelect(t1, c1);

				SRA_t *natj = malloc(sizeof(SRA_t));
				natj = SRANaturalJoin(s1,t2);

				// change pointers
				sra_select->project.sra = natj;
			}
			else if(!strcmp(join->binary.sra2->table.ref->table_name, table1))
			{
				// make copy of cond struct
				Condition_t *c1 = malloc(sizeof(Condition_t));
				memcpy(c1, select->select.cond, sizeof(Condition_t));

				// make copies of the two SRA table structs too
				SRA_t *t1 = malloc(sizeof(SRA_t));
				SRA_t *t2 = malloc(sizeof(SRA_t));
				memcpy(t1, join->binary.sra1, sizeof(SRA_t));
				memcpy(t2, join->binary.sra2, sizeof(SRA_t));

				// make new NATURAL JOIN STRUCT
				SRA_t *s2 = malloc(sizeof(SRA_t));
				s2 = SRASelect(t2, c1);

				SRA_t *natj = malloc(sizeof(SRA_t));
				natj = SRANaturalJoin(t1,s2);

				// change pointers
				sra_select->project.sra = natj;
			}
		}
		// if the two tables reference two separate tables
		// do nothing
	}

	return CHIDB_OK;
}


// // moved and edited from util.c bc the util.c ftn kept giving nulls when there should have been stuff...
// int chidb_get_where_tables(Condition_t *cond, char *first, char *second)
// {
// 	Expression_t *e1 = cond->cond.comp.expr1;
//     Expression_t *e2 = cond->cond.comp.expr2;

//     int size = 2;

//     if(e1->t == EXPR_TERM)
//     {
//         if(e1->expr.term.t == TERM_COLREF)
//         {
//         	strncpy(first, e1->expr.term.ref->tableName, 15);
//             // list_append(&tables, e1->expr.term.ref->tableName);
//         }
//     }
//     else
//     {
//     	free(first);
//     	first = NULL;
//     	size--;
//     }

//     if(e2->t == EXPR_TERM)
//     {
//         if(e2->expr.term.t == TERM_COLREF)
//         {
//         	strncpy(first, e2->expr.term.ref->tableName, 15);
//             // list_append(&tables, e2->expr.term.ref->tableName);
//         }
//     }
//     else
//     {
//     	free(second);
//     	second = NULL;
//     	size--;
//     }
//     return size;
// }


// this assumes that the col is of the same type as its comparator
int chidb_sigma_push_SRA(chidb_stmt *stmt, SRA_t *sra, Condition_t *cond)
{
	int ret1, ret2;

	if (sra->t == SRA_TABLE) // table. Have to push to this one if possible.
	{
		// push if it is the minimal table
		chidb_sigma_push_one_cond(stmt, sra, cond);
	}
	else if (sra->t == SRA_PROJECT) // project
	{
		ret1 = chidb_sigma_push_SRA(stmt, sra->project.sra, cond);
		if (ret1 == CHIDB_DELETE)
		{
			// need to delete the current select stmt...
			memcpy(sra->project.sra, sra->project.sra->select.sra, sizeof(SRA_t));
		}
		else if (ret1 != CHIDB_OK) // have to push here!
		{
			// push to this if possible. If not, return could not push
			chidb_sigma_push_one_cond(stmt, sra->project.sra, cond);
		}
		else
		{
			// return
			return CHIDB_OK;
		}

	}
	else if (sra->t == SRA_SELECT) // select. only this struct can return CHIDB_DELETE!!!
	{
		// first try and push down the cond on the select we came upon
		ret1 = chidb_sigma_push_SRA(stmt, sra->select.sra, sra->select.cond); // this is the cond on the select
		ret2 = chidb_sigma_push_SRA(stmt, sra->select.sra, cond); // this is the cond from above... see if can push lower

		if (ret1 == CHIDB_OK)
		{
			// then it worked. try and push this cond too!
			if (ret2 == CHIDB_OK)
				return CHIDB_DELETE;
			else
			{
				memcpy(sra->select.cond, cond, sizeof(Condition_t));
				// sra->select.cond = cond;
			}
		}
		else
		{
			if (ret2 != CHIDB_OK) // put them together!!!
			{
				// c->cond.binary.cond1 = cond;
				// c->cond.binary.cond2 = sra->select.cond;
				// sra->select.cond = c;
				Condition_t *c = malloc(sizeof(Condition_t));
				c->t = RA_COND_AND;
				memcpy(c->cond.binary.cond1, cond, sizeof(Condition_t));
				memcpy(c->cond.binary.cond2, sra->select.cond, sizeof(Condition_t));
				memcpy(sra->select.cond, c, sizeof(Condition_t));
			}
		}
	}
	else if (sra->t >= SRA_NATURAL_JOIN || sra->t <= SRA_RIGHT_OUTER_JOIN) // join
	{
		ret1 = chidb_sigma_push_SRA(stmt, sra->join.sra1, cond);
		ret2 = chidb_sigma_push_SRA(stmt, sra->join.sra2, cond);

		if (ret1 == CHIDB_DELETE)
		{
			memcpy(sra->join.sra1, sra->join.sra1->select.sra, sizeof(SRA_t));
			// sra->join.sra1 = sra->join.sra1->select.sra;
		}
		else if (ret1 != CHIDB_OK && ret2 != CHIDB_OK)
		{
			// have to push here
			chidb_sigma_push_one_cond(stmt, sra, cond);
		}

		if (ret2 == CHIDB_DELETE)
		{
			memcpy(sra->join.sra2, sra->join.sra2->select.sra, sizeof(SRA_t));
			// sra->join.sra2 = sra->join.sra2->select.sra;
		}
	}
	else // binary
	{
		ret1 = chidb_sigma_push_SRA(stmt, sra->binary.sra1, cond);
		ret2 = chidb_sigma_push_SRA(stmt, sra->binary.sra2, cond);

		if (ret1 == CHIDB_DELETE)
		{
			memcpy(sra->binary.sra1, sra->binary.sra1->select.sra, sizeof(SRA_t));
			// sra->binary.sra1 = sra->binary.sra1->select.sra;
		}
		else if (ret1 != CHIDB_OK && ret2 != CHIDB_OK)
		{
			chidb_sigma_push_one_cond(stmt, sra, cond);
		}

		if (ret2 == CHIDB_DELETE)
		{
			memcpy(sra->binary.sra2, sra->binary.sra2->select.sra, sizeof(SRA_t));
			// sra->binary.sra2 = sra->binary.sra2->select.sra;
		}
	}

	return CHIDB_OK;
}

/*
 * Can return: CHIDB_OK, CHIDB_EINVALIDSQL, CHIDB_TRUE
 * CHIDB_OK means that out_cond has a (possibly) simplified condition ready to go
 * CHIDB_EINVALIDSQL means gtfo! poorly formed condition!
 * CHIDB_TRUE means that the condition evaluated to true and that out_cond is NULL cause the condition should be deleted
 * NOTE: out_cond should not be malloc'd
 */
int chidb_sigma_cond_simplify(Condition_t *cond, Condition_t **out_cond)
{
	int ret1, ret2;
	Condition_t *c = NULL;
	Condition_t *v = NULL;
	*out_cond = malloc(sizeof(Condition_t));

	if (cond->t <= RA_COND_GEQ)
	{
		ret1 = chidb_sigma_evaluate(cond);

		if (ret1 == CHIDB_TRUE)
			return CHIDB_TRUE;
		else if (ret1 == CHIDB_OK)
		{
			memcpy(*out_cond, cond, sizeof(Condition_t));
			// Condition_print(*out_cond);
			// out_cond = cond;
			return CHIDB_OK;
		}
		else if (ret1 == CHIDB_FALSE)
			return CHIDB_EINVALIDSQL;
	}
	else if (cond->t == RA_COND_AND || cond->t == RA_COND_OR)
	{
		fprintf(stderr, "%s\n", "IN AND");
		ret1 = chidb_sigma_cond_simplify(cond->cond.binary.cond1, &c);
		ret2 = chidb_sigma_cond_simplify(cond->cond.binary.cond2, &v);

		if (ret1 == CHIDB_TRUE)
		{
			memcpy(*out_cond, cond->cond.binary.cond2, sizeof(Condition_t));
			// out_cond = v;
			// Condition_free(v);
			return ret2;
		}
		else if (ret1 == CHIDB_EINVALIDSQL || ret2 == CHIDB_EINVALIDSQL)
		{
			return CHIDB_EINVALIDSQL;
		}
		else // ret1 was CHIDB_OK
		{
			// Condition_free(v);
			if (ret2 == CHIDB_TRUE)
			{
				memcpy(*out_cond, c, sizeof(Condition_t));
				return ret1;
			}
			else if (ret2 == CHIDB_OK)
			{
				// out_cond = cond;
				memcpy(*out_cond, cond, sizeof(Condition_t));
				return CHIDB_OK;
			}
		}
	}
	else 
	{
		memcpy(*out_cond, cond, sizeof(Condition_t));
		// out_cond = cond;
		return CHIDB_OK;
	}

	return CHIDB_OK; // should not ever be reached!
}

int chidb_sigma_evaluate(Condition_t *cond)
{
	Expression_t *e1 = NULL;
	Expression_t *e2 = NULL;

	switch(cond->t)
	{
		case RA_COND_EQ:
		{
			e1 = cond->cond.comp.expr1;
			e2 = cond->cond.comp.expr2;

			if ((e1->t == EXPR_TERM) && (e2->t == EXPR_TERM))
			{
				if ((e1->expr.term.t != TERM_LITERAL) || (e2->expr.term.t != TERM_LITERAL))
					return CHIDB_OK;

				if (e1->expr.term.val->t != e2->expr.term.val->t)
					return CHIDB_EINVALIDSQL;

				switch(e1->expr.term.val->t)
				{
					case TYPE_INT:
						return (e1->expr.term.val->val.ival == e2->expr.term.val->val.ival) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_DOUBLE:
						return (e1->expr.term.val->val.dval == e2->expr.term.val->val.dval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_CHAR:
						return (e1->expr.term.val->val.cval == e2->expr.term.val->val.cval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_TEXT:
						return (!strncmp(e1->expr.term.val->val.strval, e1->expr.term.val->val.strval, 
							strlen(e1->expr.term.val->val.strval))) ? CHIDB_TRUE : CHIDB_FALSE;
						break;
				}
			}
		}
		break;

		case RA_COND_LT:
		{
			e1 = cond->cond.comp.expr1;
			e2 = cond->cond.comp.expr2;

			if ((e1->t == EXPR_TERM) && (e2->t == EXPR_TERM))
			{
				if ((e1->expr.term.t != TERM_LITERAL) || (e2->expr.term.t != TERM_LITERAL))
					return CHIDB_OK;

				if (e1->expr.term.val->t != e2->expr.term.val->t)
					return CHIDB_EINVALIDSQL;

				switch(e1->expr.term.val->t)
				{
					case TYPE_INT:
						return (e1->expr.term.val->val.ival < e2->expr.term.val->val.ival) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_DOUBLE:
						return (e1->expr.term.val->val.dval < e2->expr.term.val->val.dval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_CHAR:
						return (e1->expr.term.val->val.cval < e2->expr.term.val->val.cval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_TEXT:
						return (strncmp(e1->expr.term.val->val.strval, e1->expr.term.val->val.strval, 
							strlen(e1->expr.term.val->val.strval)) < 0) ? CHIDB_TRUE : CHIDB_FALSE;
						break;
				}
			}
		}
		break;

		case RA_COND_LEQ:
		{
			e1 = cond->cond.comp.expr1;
			e2 = cond->cond.comp.expr2;

			if ((e1->t == EXPR_TERM) && (e2->t == EXPR_TERM))
			{
				if ((e1->expr.term.t != TERM_LITERAL) || (e2->expr.term.t != TERM_LITERAL))
					return CHIDB_OK;

				if (e1->expr.term.val->t != e2->expr.term.val->t)
					return CHIDB_EINVALIDSQL;

				switch(e1->expr.term.val->t)
				{
					case TYPE_INT:
						return (e1->expr.term.val->val.ival <= e2->expr.term.val->val.ival) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_DOUBLE:
						return (e1->expr.term.val->val.dval <= e2->expr.term.val->val.dval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_CHAR:
						return (e1->expr.term.val->val.cval <= e2->expr.term.val->val.cval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_TEXT:
						return (strncmp(e1->expr.term.val->val.strval, e1->expr.term.val->val.strval, 
							strlen(e1->expr.term.val->val.strval)) <= 0) ? CHIDB_TRUE : CHIDB_FALSE;
						break;
				}
			}
		}
		break;

		case RA_COND_GT:
		{
			e1 = cond->cond.comp.expr1;
			e2 = cond->cond.comp.expr2;

			if ((e1->t == EXPR_TERM) && (e2->t == EXPR_TERM))
			{
				if ((e1->expr.term.t != TERM_LITERAL) || (e2->expr.term.t != TERM_LITERAL))
					return CHIDB_OK;

				if (e1->expr.term.val->t != e2->expr.term.val->t)
					return CHIDB_EINVALIDSQL;

				switch(e1->expr.term.val->t)
				{
					case TYPE_INT:
						return (e1->expr.term.val->val.ival > e2->expr.term.val->val.ival) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_DOUBLE:
						return (e1->expr.term.val->val.dval > e2->expr.term.val->val.dval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_CHAR:
						return (e1->expr.term.val->val.cval > e2->expr.term.val->val.cval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_TEXT:
						return (strncmp(e1->expr.term.val->val.strval, e1->expr.term.val->val.strval, 
							strlen(e1->expr.term.val->val.strval)) > 0) ? CHIDB_TRUE : CHIDB_FALSE;
						break;
				}
			}
		}
		break;

		case RA_COND_GEQ:
		{
			e1 = cond->cond.comp.expr1;
			e2 = cond->cond.comp.expr2;

			if ((e1->t == EXPR_TERM) && (e2->t == EXPR_TERM))
			{
				if ((e1->expr.term.t != TERM_LITERAL) || (e2->expr.term.t != TERM_LITERAL))
					return CHIDB_OK;

				if (e1->expr.term.val->t != e2->expr.term.val->t)
					return CHIDB_EINVALIDSQL;

				switch(e1->expr.term.val->t)
				{
					case TYPE_INT:
						return (e1->expr.term.val->val.ival >= e2->expr.term.val->val.ival) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_DOUBLE:
						return (e1->expr.term.val->val.dval >= e2->expr.term.val->val.dval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_CHAR:
						return (e1->expr.term.val->val.cval >= e2->expr.term.val->val.cval) ? CHIDB_TRUE : CHIDB_FALSE;
						break;

					case TYPE_TEXT:
						return (strncmp(e1->expr.term.val->val.strval, e1->expr.term.val->val.strval, 
							strlen(e1->expr.term.val->val.strval)) >= 0) ? CHIDB_TRUE : CHIDB_FALSE;
						break;
				}
			}
		}
		break;

		default: return CHIDB_OK;
	}

	return CHIDB_OK;
}











  // list_t column_names;
    // list_init(&column_names);
    // int col_list_sz = chidb_get_where_columns(column_names, select->select.cond);

    // // list_t conditions;
    // // list_init(&conditions);
    // // chidb_get_where_conds(conditions, select); // no need to parse the top level projection
    
    // if (col_list_sz == 0) return CHIDB_DONT_OPT;

    // if (list_sz == 0)
    // {
    //     fprintf(stderr, "%s\n", "THERE WERE NO SPECIFIC TABLES.");

    //     list_t all_tables;
    //     list_init(&all_tables);
    //     list_attributes_seeker(&all_tables, nameSeeker);
    //     chidb_get_sra_tables(all_tables, sra_select);

    //     list_t found_tables;
    //     list_init(&found_tables);

    //     list_iterator_start(&column_names);
    //     while(list_iterator_hasnext(&column_names))
    //     {
    //     	char *col = (char *) list_iterator_next(&column_names);
    //     	if (list_iterator_hasnext(&column_names))
    //     	{
    //     		char *col2 = (char *) list_iterator_next(&column_names);
    //     		int size = chidb_columns_in_same_table(list_t s, list_t tables, found_tables, col, col2);
    //     	}
    //     }
    //     list_iterator_stop(&column_names);

    //     if (size > 0)
    //     {
    //     	list_iterator_start(&)
    //     }


    //     list_iterator_start(&conditions);
    //     while(list_iterator_hasnext(&conditions))
    //     {
    //         Expression_t *e1 = (Expression_t *) list_iterator_next(&conditions);

    //         if (list_iterator_hasnext(&conditions))
    //         {
    //             Expression_t *e2 = (Expression_t *) list_iterator_next(&conditions);

    //             // then check types of e1 and e2. if both not col refs, do nothing or maybe eliminate...
    //             // if both colrefs, check table ownership. and check types


    //         }
    //     }
    // }