/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 268 of yacc.c  */
#line 1 "src/libchisql/sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <chidb/chidb.h>
#include <chisql/chisql.h>
#include "sql-lexer.h"

#define YYERROR_VERBOSE

void yyerror(const char *s);
int yylex(void);
extern int yylineno;
#define YYDEBUG 0
int yydebug=0;
int to_print = 0;
int num_stmts = 0;

chisql_statement_t *__stmt;



/* Line 268 of yacc.c  */
#line 94 "src/libchisql/sql-parser.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


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

/* Line 293 of yacc.c  */
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



/* Line 293 of yacc.c  */
#line 302 "src/libchisql/sql-parser.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 343 of yacc.c  */
#line 314 "src/libchisql/sql-parser.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   273

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  86
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  142
/* YYNRULES -- Number of states.  */
#define YYNSTATES  248

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   328

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      75,    76,    83,    81,    77,    82,    85,    84,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    74,
      80,    78,    79,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     8,    11,    15,    17,    19,    21,
      23,    24,    26,    28,    38,    40,    41,    43,    51,    53,
      57,    61,    63,    65,    67,    69,    71,    76,    79,    80,
      82,    86,    92,    99,   102,   108,   110,   111,   113,   116,
     119,   121,   124,   128,   131,   133,   136,   138,   142,   144,
     146,   148,   156,   160,   162,   163,   165,   167,   170,   173,
     174,   176,   177,   180,   184,   188,   193,   198,   200,   204,
     208,   211,   215,   218,   223,   228,   230,   232,   234,   236,
     238,   240,   242,   244,   247,   252,   256,   260,   262,   266,
     270,   274,   276,   280,   283,   285,   287,   289,   291,   296,
     298,   302,   305,   307,   308,   310,   312,   314,   316,   318,
     320,   322,   324,   326,   328,   333,   338,   340,   341,   344,
     349,   352,   356,   360,   364,   367,   369,   371,   373,   376,
     379,   381,   382,   391,   395,   396,   398,   402,   404,   408,
     410,   412,   414
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
      87,     0,    -1,    88,    -1,    87,    88,    -1,    89,    74,
      -1,    69,    89,    74,    -1,    90,    -1,   105,    -1,   137,
      -1,   142,    -1,    -1,    94,    -1,    91,    -1,     3,    92,
      68,    93,    51,   129,    75,   128,    76,    -1,    49,    -1,
      -1,    70,    -1,     3,     4,   129,    75,    95,    98,    76,
      -1,    96,    -1,    95,    77,    96,    -1,   128,    97,   102,
      -1,    28,    -1,    30,    -1,    31,    -1,    32,    -1,    33,
      -1,    97,    75,    73,    76,    -1,    77,    99,    -1,    -1,
     100,    -1,    99,    77,   100,    -1,    11,    13,    75,   139,
      76,    -1,    12,    13,    75,   128,    76,   101,    -1,    23,
     129,    -1,    23,   129,    75,   128,    76,    -1,   103,    -1,
      -1,   104,    -1,   104,   103,    -1,    16,    17,    -1,    49,
      -1,    11,    13,    -1,    12,    13,   101,    -1,    14,   141,
      -1,    46,    -1,    15,   114,    -1,   107,    -1,   105,   106,
     107,    -1,    43,    -1,    57,    -1,    58,    -1,     7,   108,
     119,     8,   130,   110,   109,    -1,    75,   107,    76,    -1,
      59,    -1,    -1,   113,    -1,   112,    -1,   113,   112,    -1,
     112,   113,    -1,    -1,   111,    -1,    -1,     9,   114,    -1,
      67,    25,   120,    -1,    24,    25,   120,    -1,    24,    25,
     120,    47,    -1,    24,    25,   120,    48,    -1,   115,    -1,
     115,   117,   114,    -1,   120,   118,   120,    -1,   120,   116,
      -1,    75,   114,    76,    -1,    16,   115,    -1,    50,    75,
     140,    76,    -1,    50,    75,   105,    76,    -1,    18,    -1,
      19,    -1,    78,    -1,    79,    -1,    80,    -1,    21,    -1,
      22,    -1,    20,    -1,   120,   125,    -1,   119,    77,   120,
     125,    -1,   120,    81,   121,    -1,   120,    82,   121,    -1,
     121,    -1,   121,    83,   122,    -1,   121,    84,   122,    -1,
     121,    60,   122,    -1,   122,    -1,    75,   120,    76,    -1,
      82,   122,    -1,   123,    -1,   141,    -1,    17,    -1,   124,
      -1,   126,    75,   120,    76,    -1,   127,    -1,   129,    85,
     127,    -1,    27,    70,    -1,    70,    -1,    -1,    52,    -1,
      53,    -1,    54,    -1,    55,    -1,    56,    -1,    83,    -1,
     128,    -1,    70,    -1,    70,    -1,   133,    -1,   130,   135,
     133,   131,    -1,   130,   134,   133,   131,    -1,   132,    -1,
      -1,    51,   114,    -1,    34,    75,   139,    76,    -1,   129,
     125,    -1,    39,   136,    36,    -1,    40,   136,    36,    -1,
      10,   136,    36,    -1,    41,    36,    -1,    44,    -1,    77,
      -1,    36,    -1,    42,    36,    -1,    37,    36,    -1,    38,
      -1,    -1,     5,     6,   129,   138,    45,    75,   140,    76,
      -1,    75,   139,    76,    -1,    -1,   128,    -1,   139,    77,
     128,    -1,   141,    -1,   140,    77,   141,    -1,    73,    -1,
      72,    -1,    71,    -1,    26,     8,   129,   111,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    89,    89,    90,    94,    95,    99,   100,   101,   102,
     103,   107,   108,   112,   120,   121,   125,   129,   136,   137,
     141,   149,   150,   151,   152,   153,   154,   166,   167,   171,
     172,   176,   178,   182,   183,   187,   188,   192,   198,   205,
     206,   207,   208,   209,   210,   211,   215,   216,   225,   226,
     227,   231,   242,   246,   247,   251,   252,   253,   254,   255,
     259,   260,   264,   268,   272,   273,   274,   278,   279,   287,
     296,   297,   298,   302,   303,   310,   311,   315,   316,   317,
     318,   319,   320,   324,   325,   329,   330,   331,   335,   336,
     337,   338,   342,   343,   344,   348,   349,   350,   351,   355,
     356,   361,   362,   363,   367,   368,   369,   370,   371,   375,
     376,   380,   384,   388,   389,   390,   412,   413,   417,   418,
     422,   426,   427,   428,   429,   430,   434,   434,   434,   434,
     438,   439,   443,   450,   451,   455,   456,   460,   461,   469,
     470,   471,   481
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "CREATE", "TABLE", "INSERT", "INTO",
  "SELECT", "FROM", "WHERE", "FULL", "PRIMARY", "FOREIGN", "KEY",
  "DEFAULT", "CHECK", "NOT", "TOKEN_NULL", "AND", "OR", "NEQ", "GEQ",
  "LEQ", "REFERENCES", "ORDER", "BY", "DELETE", "AS", "INT", "BYTE",
  "DOUBLE", "CHAR", "VARCHAR", "TEXT", "USING", "CONSTRAINT", "JOIN",
  "INNER", "OUTER", "LEFT", "RIGHT", "NATURAL", "CROSS", "UNION", "BOWTIE",
  "VALUES", "AUTO_INCREMENT", "ASC", "DESC", "UNIQUE", "IN", "ON", "COUNT",
  "SUM", "AVG", "MIN", "MAX", "INTERSECT", "EXCEPT", "DISTINCT", "CONCAT",
  "TRUE", "FALSE", "CASE", "WHEN", "DECLARE", "BIT", "GROUP", "INDEX",
  "EXPLAIN", "IDENTIFIER", "STRING_LITERAL", "DOUBLE_LITERAL",
  "INT_LITERAL", "';'", "'('", "')'", "','", "'='", "'>'", "'<'", "'+'",
  "'-'", "'*'", "'/'", "'.'", "$accept", "sql_queries", "sql_query",
  "sql_line", "create", "create_index", "opt_unique", "index_name",
  "create_table", "column_dec_list", "column_dec", "column_type",
  "opt_key_dec_list", "key_dec_list", "key_dec", "references_stmt",
  "opt_constraints", "constraints", "constraint", "select", "select_combo",
  "select_statement", "opt_distinct", "opt_options", "opt_where_condition",
  "where_condition", "group_by", "order_by", "condition", "bool_term",
  "in_statement", "bool_op", "comp_op", "expression_list", "expression",
  "mulexp", "primary", "term", "column_reference", "opt_alias",
  "function_name", "column_name_or_star", "column_name", "table_name",
  "table", "opt_join_condition", "join_condition", "table_ref", "join",
  "default_join", "opt_outer", "insert_into", "opt_column_names",
  "column_names_list", "values_list", "literal_value", "delete_from", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,    59,    40,    41,    44,    61,    62,
      60,    43,    45,    42,    47,    46
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    86,    87,    87,    88,    88,    89,    89,    89,    89,
      89,    90,    90,    91,    92,    92,    93,    94,    95,    95,
      96,    97,    97,    97,    97,    97,    97,    98,    98,    99,
      99,   100,   100,   101,   101,   102,   102,   103,   103,   104,
     104,   104,   104,   104,   104,   104,   105,   105,   106,   106,
     106,   107,   107,   108,   108,   109,   109,   109,   109,   109,
     110,   110,   111,   112,   113,   113,   113,   114,   114,   115,
     115,   115,   115,   116,   116,   117,   117,   118,   118,   118,
     118,   118,   118,   119,   119,   120,   120,   120,   121,   121,
     121,   121,   122,   122,   122,   123,   123,   123,   123,   124,
     124,   125,   125,   125,   126,   126,   126,   126,   126,   127,
     127,   128,   129,   130,   130,   130,   131,   131,   132,   132,
     133,   134,   134,   134,   134,   134,   135,   135,   135,   135,
     136,   136,   137,   138,   138,   139,   139,   140,   140,   141,
     141,   141,   142
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     2,     3,     1,     1,     1,     1,
       0,     1,     1,     9,     1,     0,     1,     7,     1,     3,
       3,     1,     1,     1,     1,     1,     4,     2,     0,     1,
       3,     5,     6,     2,     5,     1,     0,     1,     2,     2,
       1,     2,     3,     2,     1,     2,     1,     3,     1,     1,
       1,     7,     3,     1,     0,     1,     1,     2,     2,     0,
       1,     0,     2,     3,     3,     4,     4,     1,     3,     3,
       2,     3,     2,     4,     4,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     4,     3,     3,     1,     3,     3,
       3,     1,     3,     2,     1,     1,     1,     1,     4,     1,
       3,     2,     1,     0,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     4,     4,     1,     0,     2,     4,
       2,     3,     3,     3,     2,     1,     1,     1,     2,     2,
       1,     0,     8,     3,     0,     1,     3,     1,     3,     1,
       1,     1,     4
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      10,    15,     0,    54,     0,    10,     0,    10,     2,     0,
       6,    12,    11,     7,    46,     8,     9,     0,    14,     0,
       0,    53,     0,     0,     0,     0,     1,     3,     4,    48,
      49,    50,     0,   112,     0,     0,   134,    96,   104,   105,
     106,   107,   108,   111,   141,   140,   139,     0,     0,   109,
       0,   103,    87,    91,    94,    97,     0,    99,   110,     0,
      95,     0,     5,    52,    47,     0,    16,     0,     0,     0,
       0,    93,     0,     0,     0,   102,     0,     0,    83,     0,
       0,     0,     0,     0,     0,   142,   111,    28,    18,     0,
       0,   135,     0,     0,    92,   103,    61,   113,   103,   101,
      85,    86,    90,    88,    89,     0,   100,     0,     0,    62,
      67,     0,     0,     0,    21,    22,    23,    24,    25,    36,
       0,   133,     0,     0,   120,   131,   127,     0,   131,   131,
       0,     0,   125,   126,    59,    60,     0,     0,    84,    98,
      72,     0,     0,    75,    76,     0,    82,    80,    81,     0,
      77,    78,    79,    70,     0,     0,     0,    19,    27,    29,
      17,     0,     0,     0,     0,     0,    44,    40,     0,    20,
      35,    37,     0,   136,     0,   137,   130,     0,   129,     0,
       0,   124,   128,     0,     0,    51,    56,    55,   117,   117,
      71,    68,     0,    69,     0,     0,     0,    41,     0,    43,
      45,    39,     0,    38,     0,   132,     0,   123,   121,   122,
       0,     0,    58,    57,     0,     0,   115,   116,   114,     0,
       0,     0,     0,    30,     0,    42,    26,    13,   138,    64,
      63,     0,   118,    74,    73,     0,     0,    33,    65,    66,
       0,    31,     0,     0,   119,    32,     0,    34
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     7,     8,     9,    10,    11,    19,    67,    12,    87,
      88,   119,   113,   158,   159,   225,   169,   170,   171,    13,
      32,    14,    22,   185,   134,    85,   186,   187,   109,   110,
     153,   145,   154,    50,   111,    52,    53,    54,    55,    78,
      56,    57,    58,    59,    96,   216,   217,    97,   136,   137,
     177,    15,    69,    92,   174,    60,    16
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -184
static const yytype_int16 yypact[] =
{
       9,     5,    26,   -29,    43,    14,    11,     8,  -184,   -35,
    -184,  -184,  -184,    45,  -184,  -184,  -184,     0,  -184,    42,
       0,  -184,   122,     0,     1,    25,  -184,  -184,  -184,  -184,
    -184,  -184,    11,  -184,    41,    38,    68,  -184,  -184,  -184,
    -184,  -184,  -184,    61,  -184,  -184,  -184,   122,   122,  -184,
       2,    15,    34,  -184,  -184,  -184,    94,  -184,  -184,    88,
    -184,   172,  -184,  -184,  -184,   113,  -184,   140,   113,   158,
      24,  -184,     0,   122,   136,  -184,   122,   122,  -184,   122,
     122,   122,   122,   -50,    82,  -184,  -184,   134,  -184,   187,
       0,  -184,    48,   141,  -184,    -5,    27,  -184,    15,  -184,
      34,    34,  -184,  -184,  -184,    31,  -184,    82,    82,  -184,
     111,   106,    12,   151,  -184,  -184,  -184,  -184,  -184,   133,
     153,  -184,   113,    50,  -184,   191,  -184,   194,   191,   191,
     195,   196,  -184,  -184,     7,  -184,     0,     0,  -184,  -184,
    -184,   157,   120,  -184,  -184,    82,  -184,  -184,  -184,   159,
    -184,  -184,  -184,  -184,   122,   222,   223,  -184,   160,  -184,
    -184,   225,   226,    50,    82,   224,  -184,  -184,   167,  -184,
    -184,   198,   113,  -184,    55,  -184,  -184,   206,  -184,   207,
     209,  -184,  -184,   221,   227,  -184,   229,   181,    -6,    -6,
    -184,  -184,    20,    69,   174,   175,   149,  -184,   228,  -184,
    -184,  -184,   178,  -184,   179,  -184,    50,  -184,  -184,  -184,
     122,   122,  -184,  -184,   182,    82,  -184,  -184,  -184,   -14,
      86,   113,   113,  -184,     0,  -184,  -184,  -184,  -184,    33,
      69,   113,  -184,  -184,  -184,    90,   180,   183,  -184,  -184,
      95,  -184,   228,   113,  -184,  -184,   184,  -184
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -184,  -184,   252,   256,  -184,  -184,  -184,  -184,  -184,  -184,
     150,  -184,  -184,  -184,    67,    22,  -184,    96,  -184,    73,
    -184,    40,  -184,  -184,  -184,   170,    81,    83,  -104,   163,
    -184,  -184,  -184,  -184,   -21,   145,   -23,  -184,  -184,   -45,
    -184,   188,   -63,   -17,  -184,    84,  -184,    87,  -184,  -184,
      97,  -184,  -184,  -183,    80,  -116,  -184
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -113
static const yytype_int16 yytable[] =
{
      34,    51,    89,    36,   141,    91,    61,   175,    26,    17,
      72,     1,     1,     2,     2,     3,     3,     1,     3,     2,
      86,     3,    74,   155,   156,    71,    70,     3,   214,    29,
      21,   183,    20,    49,     4,     4,    84,   125,   235,    28,
       4,   191,    74,    30,    31,   215,    25,   199,   240,    89,
     124,    23,    98,   138,    18,    95,   102,   103,   104,   173,
     200,   105,   233,   126,   127,    75,   128,   129,   130,   131,
      33,   132,    64,   120,   184,    62,   175,     5,     5,    73,
     238,   239,    86,     6,     6,    75,     6,   142,    29,     6,
     228,    44,    45,    46,    79,     6,    76,    77,   107,    37,
      94,    63,    30,    31,   133,    76,    77,   139,    66,   204,
      35,   232,    76,    77,    76,    77,    65,    80,    81,    95,
      95,    44,    45,    46,   121,   122,   146,   147,   148,   143,
     144,   205,   206,   193,    38,    39,    40,    41,    42,    37,
     146,   147,   148,    68,   161,   162,  -112,   163,   164,   165,
      76,    77,    43,    44,    45,    46,   149,   108,    91,   236,
     155,   156,   234,   206,    48,    49,   241,   122,    91,    82,
     149,   244,   122,    83,    38,    39,    40,    41,    42,   166,
     246,    84,   167,    86,   150,   151,   152,    76,    77,   229,
     230,    90,    43,    44,    45,    46,    94,    47,   150,   151,
     152,    76,    77,    93,    48,    49,    99,   237,   168,   161,
     162,   112,   163,   164,   165,   114,   123,   115,   116,   117,
     118,   100,   101,   188,   189,   179,   180,   160,   172,   176,
     178,   181,   182,   190,   192,   194,   195,   196,   197,   198,
     202,   201,   207,   208,   166,   209,   210,   167,   184,   221,
     222,   224,   211,   183,   226,   227,   242,   231,   243,    27,
     247,    24,   157,   223,   245,   219,   135,   203,   213,   212,
     140,   106,   220,   218
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-184))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_uint8 yycheck[] =
{
      17,    22,    65,    20,   108,    68,    23,   123,     0,     4,
       8,     3,     3,     5,     5,     7,     7,     3,     7,     5,
      70,     7,    27,    11,    12,    48,    47,     7,    34,    43,
      59,    24,     6,    83,    26,    26,     9,    10,   221,    74,
      26,   145,    27,    57,    58,    51,     6,   163,   231,   112,
      95,     8,    73,    98,    49,    72,    79,    80,    81,   122,
     164,    82,    76,    36,    37,    70,    39,    40,    41,    42,
      70,    44,    32,    90,    67,    74,   192,    69,    69,    77,
      47,    48,    70,    75,    75,    70,    75,   108,    43,    75,
     206,    71,    72,    73,    60,    75,    81,    82,    16,    17,
      76,    76,    57,    58,    77,    81,    82,    76,    70,   172,
      68,   215,    81,    82,    81,    82,    75,    83,    84,   136,
     137,    71,    72,    73,    76,    77,    20,    21,    22,    18,
      19,    76,    77,   154,    52,    53,    54,    55,    56,    17,
      20,    21,    22,    75,    11,    12,    85,    14,    15,    16,
      81,    82,    70,    71,    72,    73,    50,    75,   221,   222,
      11,    12,    76,    77,    82,    83,    76,    77,   231,    75,
      50,    76,    77,    85,    52,    53,    54,    55,    56,    46,
     243,     9,    49,    70,    78,    79,    80,    81,    82,   210,
     211,    51,    70,    71,    72,    73,    76,    75,    78,    79,
      80,    81,    82,    45,    82,    83,    70,   224,    75,    11,
      12,    77,    14,    15,    16,    28,    75,    30,    31,    32,
      33,    76,    77,   136,   137,   128,   129,    76,    75,    38,
      36,    36,    36,    76,    75,    13,    13,    77,    13,    13,
      73,    17,    36,    36,    46,    36,    25,    49,    67,    75,
      75,    23,    25,    24,    76,    76,    76,    75,    75,     7,
      76,     5,   112,   196,   242,   192,    96,   171,   187,   186,
     107,    83,   192,   189
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     5,     7,    26,    69,    75,    87,    88,    89,
      90,    91,    94,   105,   107,   137,   142,     4,    49,    92,
       6,    59,   108,     8,    89,   107,     0,    88,    74,    43,
      57,    58,   106,    70,   129,    68,   129,    17,    52,    53,
      54,    55,    56,    70,    71,    72,    73,    75,    82,    83,
     119,   120,   121,   122,   123,   124,   126,   127,   128,   129,
     141,   129,    74,    76,   107,    75,    70,    93,    75,   138,
     120,   122,     8,    77,    27,    70,    81,    82,   125,    60,
      83,    84,    75,    85,     9,   111,    70,    95,    96,   128,
      51,   128,   139,    45,    76,   129,   130,   133,   120,    70,
     121,   121,   122,   122,   122,   120,   127,    16,    75,   114,
     115,   120,    77,    98,    28,    30,    31,    32,    33,    97,
     129,    76,    77,    75,   125,    10,    36,    37,    39,    40,
      41,    42,    44,    77,   110,   111,   134,   135,   125,    76,
     115,   114,   120,    18,    19,   117,    20,    21,    22,    50,
      78,    79,    80,   116,   118,    11,    12,    96,    99,   100,
      76,    11,    12,    14,    15,    16,    46,    49,    75,   102,
     103,   104,    75,   128,   140,   141,    38,   136,    36,   136,
     136,    36,    36,    24,    67,   109,   112,   113,   133,   133,
      76,   114,    75,   120,    13,    13,    77,    13,    13,   141,
     114,    17,    73,   103,   128,    76,    77,    36,    36,    36,
      25,    25,   113,   112,    34,    51,   131,   132,   131,   105,
     140,    75,    75,   100,    23,   101,    76,    76,   141,   120,
     120,    75,   114,    76,    76,   139,   128,   129,    47,    48,
     139,    76,    76,    75,    76,   101,   128,    76
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:

/* Line 1806 of yacc.c  */
#line 94 "src/libchisql/sql.y"
    { __stmt->explain = false; }
    break;

  case 5:

/* Line 1806 of yacc.c  */
#line 95 "src/libchisql/sql.y"
    { __stmt->explain = true; }
    break;

  case 6:

/* Line 1806 of yacc.c  */
#line 99 "src/libchisql/sql.y"
    { __stmt->stmt.create = (yyvsp[(1) - (1)].cre); __stmt->type = STMT_CREATE; }
    break;

  case 7:

/* Line 1806 of yacc.c  */
#line 100 "src/libchisql/sql.y"
    { __stmt->stmt.select = (yyvsp[(1) - (1)].sra); __stmt->type = STMT_SELECT; }
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 101 "src/libchisql/sql.y"
    { __stmt->stmt.insert = (yyvsp[(1) - (1)].ins); __stmt->type = STMT_INSERT; }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 102 "src/libchisql/sql.y"
    { __stmt->stmt.delete = (yyvsp[(1) - (1)].del); __stmt->type = STMT_DELETE; }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 107 "src/libchisql/sql.y"
    { (yyval.cre) = Create_fromTable((yyvsp[(1) - (1)].tbl)); }
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 108 "src/libchisql/sql.y"
    { (yyval.cre) = Create_fromIndex((yyvsp[(1) - (1)].idx)); }
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 113 "src/libchisql/sql.y"
    { 
			(yyval.idx) = Index_make((yyvsp[(4) - (9)].strval), (yyvsp[(6) - (9)].strval), (yyvsp[(8) - (9)].strval)); 
		  	if ((yyvsp[(2) - (9)].ival) == UNIQUE) (yyval.idx) = Index_makeUnique((yyval.idx)); 
		}
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 120 "src/libchisql/sql.y"
    { (yyval.ival) = UNIQUE; }
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 121 "src/libchisql/sql.y"
    { (yyval.ival) = 0; }
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 130 "src/libchisql/sql.y"
    {
			(yyval.tbl) = Table_make((yyvsp[(3) - (7)].strval), (yyvsp[(5) - (7)].col), (yyvsp[(6) - (7)].kdec));
		}
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 137 "src/libchisql/sql.y"
    { (yyval.col) = Column_append((yyvsp[(1) - (3)].col), (yyvsp[(3) - (3)].col)); }
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 142 "src/libchisql/sql.y"
    { 
			/*printf("column '%s' (%d)\n", $1, $2);*/
			(yyval.col) = Column((yyvsp[(1) - (3)].strval), (yyvsp[(2) - (3)].ival), (yyvsp[(3) - (3)].constr));
		}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 149 "src/libchisql/sql.y"
    { (yyval.ival) = TYPE_INT; }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 150 "src/libchisql/sql.y"
    { (yyval.ival) = TYPE_DOUBLE; }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 151 "src/libchisql/sql.y"
    { (yyval.ival) = TYPE_CHAR; }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 152 "src/libchisql/sql.y"
    { (yyval.ival) = TYPE_TEXT; }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 153 "src/libchisql/sql.y"
    { (yyval.ival) = TYPE_TEXT; }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 155 "src/libchisql/sql.y"
    { 
			(yyval.ival) = (yyvsp[(1) - (4)].ival);
			if ((yyvsp[(3) - (4)].ival) <= 0) {
				fprintf(stderr, "Error: sizes must be greater than 0 (line %d).\n", yylineno);
				exit(1);
			}
			Column_setSize((yyvsp[(3) - (4)].ival));
		}
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 166 "src/libchisql/sql.y"
    {(yyval.kdec) = (yyvsp[(2) - (2)].kdec);}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 167 "src/libchisql/sql.y"
    {(yyval.kdec) = NULL; }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 172 "src/libchisql/sql.y"
    { (yyval.kdec) = KeyDec_append((yyvsp[(1) - (3)].kdec), (yyvsp[(3) - (3)].kdec)); }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 177 "src/libchisql/sql.y"
    { (yyval.kdec) = PrimaryKeyDec((yyvsp[(4) - (5)].slist)); }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 179 "src/libchisql/sql.y"
    {(yyval.kdec) = ForeignKeyDec(ForeignKeyRef_makeFull((yyvsp[(4) - (6)].strval), (yyvsp[(6) - (6)].fkeyref))); }
    break;

  case 33:

/* Line 1806 of yacc.c  */
#line 182 "src/libchisql/sql.y"
    { (yyval.fkeyref) = ForeignKeyRef_make((yyvsp[(2) - (2)].strval), NULL); }
    break;

  case 34:

/* Line 1806 of yacc.c  */
#line 183 "src/libchisql/sql.y"
    { (yyval.fkeyref) = ForeignKeyRef_make((yyvsp[(2) - (5)].strval), (yyvsp[(4) - (5)].strval)); }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 188 "src/libchisql/sql.y"
    { (yyval.constr) = NULL; }
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 192 "src/libchisql/sql.y"
    { (yyval.constr) = Constraint_append(NULL, (yyvsp[(1) - (1)].constr)); 
						/*printf("new constraint:");
						Constraint_print($1);
						printf("created a vector of constraints\n");
						Constraint_printList($$);*/
					 }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 198 "src/libchisql/sql.y"
    { (yyval.constr) = Constraint_append((yyvsp[(2) - (2)].constr), (yyvsp[(1) - (2)].constr)); 
										/*printf("appended a constraint\n");
										Constraint_printList($$);*/
									}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 205 "src/libchisql/sql.y"
    { (yyval.constr) = NotNull(); }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 206 "src/libchisql/sql.y"
    { (yyval.constr) = Unique(); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 207 "src/libchisql/sql.y"
    { (yyval.constr) = PrimaryKey(); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 208 "src/libchisql/sql.y"
    { (yyval.constr) = ForeignKey((yyvsp[(3) - (3)].fkeyref)); }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 209 "src/libchisql/sql.y"
    { (yyval.constr) = Default((yyvsp[(2) - (2)].lval)); }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 210 "src/libchisql/sql.y"
    { (yyval.constr) = AutoIncrement(); }
    break;

  case 45:

/* Line 1806 of yacc.c  */
#line 211 "src/libchisql/sql.y"
    { (yyval.constr) = Check((yyvsp[(2) - (2)].cond)); }
    break;

  case 47:

/* Line 1806 of yacc.c  */
#line 217 "src/libchisql/sql.y"
    { 
			(yyval.sra) = ((yyvsp[(2) - (3)].ival) == UNION) ? SRAUnion((yyvsp[(1) - (3)].sra), (yyvsp[(3) - (3)].sra)) :
				  ((yyvsp[(2) - (3)].ival) == INTERSECT) ? SRAIntersect((yyvsp[(1) - (3)].sra), (yyvsp[(3) - (3)].sra)) :
				  SRAExcept((yyvsp[(1) - (3)].sra), (yyvsp[(3) - (3)].sra));
		}
    break;

  case 48:

/* Line 1806 of yacc.c  */
#line 225 "src/libchisql/sql.y"
    {(yyval.ival) = UNION;}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 226 "src/libchisql/sql.y"
    {(yyval.ival) = INTERSECT;}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 227 "src/libchisql/sql.y"
    {(yyval.ival) = EXCEPT;}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 232 "src/libchisql/sql.y"
    {
			if ((yyvsp[(6) - (7)].cond) != NULL) 
				(yyval.sra) = SRAProject(SRASelect((yyvsp[(5) - (7)].sra), (yyvsp[(6) - (7)].cond)), (yyvsp[(3) - (7)].expr));
			else
				(yyval.sra) = SRAProject((yyvsp[(5) - (7)].sra), (yyvsp[(3) - (7)].expr));
			if ((yyvsp[(7) - (7)].opt) != NULL)
				(yyval.sra) = SRA_applyOption((yyval.sra), (yyvsp[(7) - (7)].opt)); 
			if ((yyvsp[(2) - (7)].ival) == DISTINCT)
				(yyval.sra) = SRA_makeDistinct((yyval.sra));
		}
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 242 "src/libchisql/sql.y"
    { (yyval.sra) = (yyvsp[(2) - (3)].sra); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 246 "src/libchisql/sql.y"
    { (yyval.ival) = DISTINCT;}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 247 "src/libchisql/sql.y"
    { (yyval.ival) = 0; }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 251 "src/libchisql/sql.y"
    {(yyval.opt) = (yyvsp[(1) - (1)].opt); }
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 252 "src/libchisql/sql.y"
    {(yyval.opt) = (yyvsp[(1) - (1)].opt); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 253 "src/libchisql/sql.y"
    {(yyval.opt) = ProjectOption_combine((yyvsp[(1) - (2)].opt), (yyvsp[(2) - (2)].opt));}
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 254 "src/libchisql/sql.y"
    {(yyval.opt) = ProjectOption_combine((yyvsp[(1) - (2)].opt), (yyvsp[(2) - (2)].opt));}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 255 "src/libchisql/sql.y"
    { (yyval.opt) = NULL; }
    break;

  case 60:

/* Line 1806 of yacc.c  */
#line 259 "src/libchisql/sql.y"
    {(yyval.cond) = (yyvsp[(1) - (1)].cond);}
    break;

  case 61:

/* Line 1806 of yacc.c  */
#line 260 "src/libchisql/sql.y"
    {(yyval.cond) = NULL;}
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 264 "src/libchisql/sql.y"
    { (yyval.cond) = (yyvsp[(2) - (2)].cond); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 268 "src/libchisql/sql.y"
    { (yyval.opt) = GroupBy_make((yyvsp[(3) - (3)].expr)); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 272 "src/libchisql/sql.y"
    { (yyval.opt) = OrderBy_make((yyvsp[(3) - (3)].expr), ORDER_BY_ASC); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 273 "src/libchisql/sql.y"
    { (yyval.opt) = OrderBy_make((yyvsp[(3) - (4)].expr), ORDER_BY_ASC); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 274 "src/libchisql/sql.y"
    { (yyval.opt) = OrderBy_make((yyvsp[(3) - (4)].expr), ORDER_BY_DESC); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 278 "src/libchisql/sql.y"
    { (yyval.cond) = (yyvsp[(1) - (1)].cond); /*printf("Found condition: \n"); Condition_print($$); puts(""); */}
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 280 "src/libchisql/sql.y"
    { 
   		(yyval.cond) = ((yyvsp[(2) - (3)].ival) == AND) ? And((yyvsp[(1) - (3)].cond), (yyvsp[(3) - (3)].cond)) : Or((yyvsp[(1) - (3)].cond), (yyvsp[(3) - (3)].cond)); 
   		/* printf("Found condition: \n"); Condition_print($$); puts(""); */
   	}
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 288 "src/libchisql/sql.y"
    {
   		(yyval.cond) = ((yyvsp[(2) - (3)].ival) == '=') ? Eq((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)) :
   			  ((yyvsp[(2) - (3)].ival) == '>') ? Gt((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)) :
   			  ((yyvsp[(2) - (3)].ival) == '<') ? Lt((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)) :
   			  ((yyvsp[(2) - (3)].ival) == GEQ) ? Geq((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)) :
			  ((yyvsp[(2) - (3)].ival) == LEQ) ? Leq((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)) :
   			  Not(Eq((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)));
   	}
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 296 "src/libchisql/sql.y"
    { (yyval.cond) = In((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].lval)); }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 297 "src/libchisql/sql.y"
    { (yyval.cond) = (yyvsp[(2) - (3)].cond); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 298 "src/libchisql/sql.y"
    { (yyval.cond) = Not((yyvsp[(2) - (2)].cond)); }
    break;

  case 73:

/* Line 1806 of yacc.c  */
#line 302 "src/libchisql/sql.y"
    { (yyval.lval) = (yyvsp[(3) - (4)].lval); }
    break;

  case 74:

/* Line 1806 of yacc.c  */
#line 304 "src/libchisql/sql.y"
    {
   		fprintf(stderr, "****WARNING: IN SELECT statement not yet supported\n");
   	}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 310 "src/libchisql/sql.y"
    { (yyval.ival) = AND; }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 311 "src/libchisql/sql.y"
    { (yyval.ival) = OR; }
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 315 "src/libchisql/sql.y"
    { (yyval.ival) = '='; }
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 316 "src/libchisql/sql.y"
    { (yyval.ival) = '>'; }
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 317 "src/libchisql/sql.y"
    { (yyval.ival) = '<'; }
    break;

  case 80:

/* Line 1806 of yacc.c  */
#line 318 "src/libchisql/sql.y"
    { (yyval.ival) = GEQ; }
    break;

  case 81:

/* Line 1806 of yacc.c  */
#line 319 "src/libchisql/sql.y"
    { (yyval.ival) = LEQ; }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 320 "src/libchisql/sql.y"
    { (yyval.ival) = NEQ; }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 324 "src/libchisql/sql.y"
    { (yyval.expr) = add_alias((yyvsp[(1) - (2)].expr), (yyvsp[(2) - (2)].strval)); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 325 "src/libchisql/sql.y"
    { (yyval.expr) = append_expression((yyvsp[(1) - (4)].expr), add_alias((yyvsp[(3) - (4)].expr), (yyvsp[(4) - (4)].strval))); }
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 329 "src/libchisql/sql.y"
    { (yyval.expr) = Plus((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 330 "src/libchisql/sql.y"
    { (yyval.expr) = Minus((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 331 "src/libchisql/sql.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 335 "src/libchisql/sql.y"
    { (yyval.expr) = Multiply((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 336 "src/libchisql/sql.y"
    { (yyval.expr) = Divide((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 337 "src/libchisql/sql.y"
    { (yyval.expr) = Concat((yyvsp[(1) - (3)].expr), (yyvsp[(3) - (3)].expr)); }
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 338 "src/libchisql/sql.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 342 "src/libchisql/sql.y"
    { (yyval.expr) = (yyvsp[(2) - (3)].expr); }
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 343 "src/libchisql/sql.y"
    { (yyval.expr) = Neg((yyvsp[(2) - (2)].expr)); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 344 "src/libchisql/sql.y"
    { (yyval.expr) = (yyvsp[(1) - (1)].expr); }
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 348 "src/libchisql/sql.y"
    { (yyval.expr) = TermLiteral((yyvsp[(1) - (1)].lval)); }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 349 "src/libchisql/sql.y"
    { (yyval.expr) = TermNull(); }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 350 "src/libchisql/sql.y"
    { (yyval.expr) = TermColumnReference((yyvsp[(1) - (1)].colref)); }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 351 "src/libchisql/sql.y"
    { (yyval.expr) = TermFunction((yyvsp[(1) - (4)].ival), (yyvsp[(3) - (4)].expr)); }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 355 "src/libchisql/sql.y"
    { (yyval.colref) = ColumnReference_make(NULL, (yyvsp[(1) - (1)].strval)); }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 357 "src/libchisql/sql.y"
    { (yyval.colref) = ColumnReference_make((yyvsp[(1) - (3)].strval), (yyvsp[(3) - (3)].strval)); }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 361 "src/libchisql/sql.y"
    { (yyval.strval) = (yyvsp[(2) - (2)].strval); }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 363 "src/libchisql/sql.y"
    { (yyval.strval) = NULL; }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 367 "src/libchisql/sql.y"
    { (yyval.ival) = FUNC_COUNT; }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 368 "src/libchisql/sql.y"
    { (yyval.ival) = FUNC_SUM; }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 369 "src/libchisql/sql.y"
    { (yyval.ival) = FUNC_AVG; }
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 370 "src/libchisql/sql.y"
    { (yyval.ival) = FUNC_MIN; }
    break;

  case 108:

/* Line 1806 of yacc.c  */
#line 371 "src/libchisql/sql.y"
    { (yyval.ival) = FUNC_MAX; }
    break;

  case 109:

/* Line 1806 of yacc.c  */
#line 375 "src/libchisql/sql.y"
    { (yyval.strval) = strdup("*"); }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 388 "src/libchisql/sql.y"
    { (yyval.sra) = SRATable((yyvsp[(1) - (1)].tref)); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 389 "src/libchisql/sql.y"
    { (yyval.sra) = SRAJoin((yyvsp[(1) - (4)].sra), SRATable((yyvsp[(3) - (4)].tref)), (yyvsp[(4) - (4)].jcond)); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 391 "src/libchisql/sql.y"
    {
			switch ((yyvsp[(2) - (4)].ival)) {
				case SRA_NATURAL_JOIN:
					(yyval.sra) = SRANaturalJoin((yyvsp[(1) - (4)].sra), SRATable((yyvsp[(3) - (4)].tref))); 
					if ((yyvsp[(4) - (4)].jcond)) {
						fprintf(stderr, 
								  "Line %d: WARNING: a NATURAL join cannot have an ON "
								  "or USING clause. This will be ignored.\n", yylineno);
					}
					break;
				case SRA_LEFT_OUTER_JOIN:
					(yyval.sra) = SRALeftOuterJoin((yyvsp[(1) - (4)].sra), SRATable((yyvsp[(3) - (4)].tref)), (yyvsp[(4) - (4)].jcond)); break;
				case SRA_RIGHT_OUTER_JOIN:
					(yyval.sra) = SRARightOuterJoin((yyvsp[(1) - (4)].sra), SRATable((yyvsp[(3) - (4)].tref)), (yyvsp[(4) - (4)].jcond)); break;
				case SRA_FULL_OUTER_JOIN:
					(yyval.sra) = SRAFullOuterJoin((yyvsp[(1) - (4)].sra), SRATable((yyvsp[(3) - (4)].tref)), (yyvsp[(4) - (4)].jcond)); break;
			}
		}
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 413 "src/libchisql/sql.y"
    { (yyval.jcond) = NULL; }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 417 "src/libchisql/sql.y"
    { (yyval.jcond) = On((yyvsp[(2) - (2)].cond)); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 418 "src/libchisql/sql.y"
    { (yyval.jcond) = Using((yyvsp[(3) - (4)].slist)); }
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 422 "src/libchisql/sql.y"
    { (yyval.tref) = TableReference_make((yyvsp[(1) - (2)].strval), (yyvsp[(2) - (2)].strval));}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 426 "src/libchisql/sql.y"
    {(yyval.ival) = SRA_LEFT_OUTER_JOIN; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 427 "src/libchisql/sql.y"
    { (yyval.ival) = SRA_RIGHT_OUTER_JOIN; }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 428 "src/libchisql/sql.y"
    { (yyval.ival) = SRA_FULL_OUTER_JOIN; }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 429 "src/libchisql/sql.y"
    { (yyval.ival) = SRA_NATURAL_JOIN; }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 430 "src/libchisql/sql.y"
    { (yyval.ival) = SRA_NATURAL_JOIN; }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 444 "src/libchisql/sql.y"
    {
			(yyval.ins) = Insert_make((yyvsp[(3) - (8)].strval), (yyvsp[(4) - (8)].slist), (yyvsp[(7) - (8)].lval));
		}
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 450 "src/libchisql/sql.y"
    { (yyval.slist) = (yyvsp[(2) - (3)].slist); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 451 "src/libchisql/sql.y"
    { (yyval.slist) = NULL; }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 455 "src/libchisql/sql.y"
    { (yyval.slist) = StrList_make((yyvsp[(1) - (1)].strval)); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 456 "src/libchisql/sql.y"
    { (yyval.slist) = StrList_append((yyvsp[(1) - (3)].slist), StrList_make((yyvsp[(3) - (3)].strval))); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 462 "src/libchisql/sql.y"
    { 
			(yyval.lval) = Literal_append((yyvsp[(1) - (3)].lval), (yyvsp[(3) - (3)].lval)); 

		}
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 469 "src/libchisql/sql.y"
    { (yyval.lval) = litInt((yyvsp[(1) - (1)].ival)); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 470 "src/libchisql/sql.y"
    { (yyval.lval) = litDouble((yyvsp[(1) - (1)].dval)); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 472 "src/libchisql/sql.y"
    {
			if (strlen((yyvsp[(1) - (1)].strval)) == 1)
				(yyval.lval) = litChar((yyvsp[(1) - (1)].strval)[0]);
			else
				(yyval.lval) = litText((yyvsp[(1) - (1)].strval));
		}
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 482 "src/libchisql/sql.y"
    {
			(yyval.del) = Delete_make((yyvsp[(3) - (4)].strval), (yyvsp[(4) - (4)].cond));
		}
    break;



/* Line 1806 of yacc.c  */
#line 2717 "src/libchisql/sql-parser.c"
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 487 "src/libchisql/sql.y"


void yyerror(const char *s) {
	fprintf(stderr, "%s (line %d)\n", s, yylineno);
}

int chisql_stmt_print(chisql_statement_t *stmt)
{
    switch(stmt->type)
    {
    case STMT_CREATE:
        Create_print(stmt->stmt.create);
        break;
    case STMT_SELECT:
        SRA_print(stmt->stmt.select);
        break;
    case STMT_INSERT:
        Insert_print(stmt->stmt.insert);
        break;
    case STMT_DELETE:
        Delete_print(stmt->stmt.delete);
        break;
    }

    return 0;
}


char *__sql_semicolon(const char *sql)
{
  int len = strlen(sql);
  char *t = strdup(sql);
  if (t[len-1]!=';') {
    t = realloc(t,len+2);
    t[len]=';';
    t[len+1]=0;
  }
  return t;
}

int chisql_parser(const char *sql, chisql_statement_t **stmt)
{
  int rc;
  
  __stmt = malloc(sizeof(chisql_statement_t));
  char *tsql = __sql_semicolon(sql);
    
  YY_BUFFER_STATE my_string_buffer = yy_scan_string (tsql);
  rc = yyparse();
  yy_delete_buffer (my_string_buffer);
  
  if (rc == 0) {
    __stmt->text = tsql; /* strdup(sql); */
    *stmt = __stmt;
    return CHIDB_OK;
  } else {
    fprintf(stderr,"invalid sql: \"%s\"\n", tsql);
    free(__stmt);
    return CHIDB_EINVALIDSQL;
  }

}

