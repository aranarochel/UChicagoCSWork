/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     CREATE = 258,
     TABLE = 259,
     INSERT = 260,
     INTO = 261,
     SELECT = 262,
     FROM = 263,
     WHERE = 264,
     FULL = 265,
     PRIMARY = 266,
     FOREIGN = 267,
     KEY = 268,
     DEFAULT = 269,
     CHECK = 270,
     NOT = 271,
     TOKEN_NULL = 272,
     AND = 273,
     OR = 274,
     NEQ = 275,
     GEQ = 276,
     LEQ = 277,
     REFERENCES = 278,
     ORDER = 279,
     BY = 280,
     DELETE = 281,
     AS = 282,
     INT = 283,
     BYTE = 284,
     DOUBLE = 285,
     CHAR = 286,
     VARCHAR = 287,
     TEXT = 288,
     USING = 289,
     CONSTRAINT = 290,
     JOIN = 291,
     INNER = 292,
     OUTER = 293,
     LEFT = 294,
     RIGHT = 295,
     NATURAL = 296,
     CROSS = 297,
     UNION = 298,
     BOWTIE = 299,
     VALUES = 300,
     AUTO_INCREMENT = 301,
     ASC = 302,
     DESC = 303,
     UNIQUE = 304,
     IN = 305,
     ON = 306,
     COUNT = 307,
     SUM = 308,
     AVG = 309,
     MIN = 310,
     MAX = 311,
     INTERSECT = 312,
     EXCEPT = 313,
     DISTINCT = 314,
     CONCAT = 315,
     TRUE = 316,
     FALSE = 317,
     CASE = 318,
     WHEN = 319,
     DECLARE = 320,
     BIT = 321,
     GROUP = 322,
     INDEX = 323,
     EXPLAIN = 324,
     IDENTIFIER = 325,
     STRING_LITERAL = 326,
     DOUBLE_LITERAL = 327,
     INT_LITERAL = 328
   };
#endif
/* Tokens.  */
#define CREATE 258
#define TABLE 259
#define INSERT 260
#define INTO 261
#define SELECT 262
#define FROM 263
#define WHERE 264
#define FULL 265
#define PRIMARY 266
#define FOREIGN 267
#define KEY 268
#define DEFAULT 269
#define CHECK 270
#define NOT 271
#define TOKEN_NULL 272
#define AND 273
#define OR 274
#define NEQ 275
#define GEQ 276
#define LEQ 277
#define REFERENCES 278
#define ORDER 279
#define BY 280
#define DELETE 281
#define AS 282
#define INT 283
#define BYTE 284
#define DOUBLE 285
#define CHAR 286
#define VARCHAR 287
#define TEXT 288
#define USING 289
#define CONSTRAINT 290
#define JOIN 291
#define INNER 292
#define OUTER 293
#define LEFT 294
#define RIGHT 295
#define NATURAL 296
#define CROSS 297
#define UNION 298
#define BOWTIE 299
#define VALUES 300
#define AUTO_INCREMENT 301
#define ASC 302
#define DESC 303
#define UNIQUE 304
#define IN 305
#define ON 306
#define COUNT 307
#define SUM 308
#define AVG 309
#define MIN 310
#define MAX 311
#define INTERSECT 312
#define EXCEPT 313
#define DISTINCT 314
#define CONCAT 315
#define TRUE 316
#define FALSE 317
#define CASE 318
#define WHEN 319
#define DECLARE 320
#define BIT 321
#define GROUP 322
#define INDEX 323
#define EXPLAIN 324
#define IDENTIFIER 325
#define STRING_LITERAL 326
#define DOUBLE_LITERAL 327
#define INT_LITERAL 328




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 2068 of yacc.c  */
#line 23 "src/libchisql/sql.y"

	double dval;
	int ival;
	char *strval;
	Literal_t *lval;
	Constraint_t *constr;
	ForeignKeyRef_t fkeyref;
	Column_t *col;
	KeyDec_t *kdec;
	StrList_t *slist;
	Insert_t *ins;
	Condition_t *cond;
	Expression_t *expr;
	ColumnReference_t *colref;
	Delete_t *del;
	SRA_t *sra;
	ProjectOption_t *opt;
	TableReference_t *tref;
	Table_t *tbl;
	JoinCondition_t *jcond;
	Index_t *idx;
	Create_t *cre;



/* Line 2068 of yacc.c  */
#line 222 "src/libchisql/sql-parser.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


