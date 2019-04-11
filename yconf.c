/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 7 "conf.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

#include "util.h"
#include "solve.h"
#include "port_pair.h"
#include "proto_map.hpp"
#include "entry.hpp"
#include "dst_addr.hpp"
#include "director.hpp"
#include "portfwd.h"

/*
 * These are from the lexical analyzer defined in conf.lex
 */
extern int yylex();
extern char yytext[];
extern void show_last_token();
extern int conf_line_number;
extern char conf_ident[];
extern char conf_lex_str_buf[];

/*
 * Some useful constants
 */
const char *const ANY_ADDR = "0.0.0.0";
const int MIN_MASK_LEN = 0;
const int MAX_MASK_LEN = 32;

/*
 * We store the number of syntax errors here
 */
int conf_syntax_errors = 0;

void yyerror(const char *msg)
{
  ++conf_syntax_errors;
  syslog(LOG_ERR, "Syntax error: %s\n", msg);
  show_last_token();
}

/* Variaveis Auxiliares */

proto_t curr_proto;
vector<from_addr*> *from_vector;
vector<to_addr*>   *dst_vector;
vector<host_map*>  *host_vector;
vector<int>        *port_vector;
vector<proto_map*> *map_vector;
vector<entry*>     *entry_vector = new vector<entry*>();

int            conf_user   = -1;
int            conf_group  = -1;
long long      conf_xor_key = 0;
long long      conf_confusing_key = 0;
int	       conf_is_remote_server = 0;

const struct ip_addr conf_any_addr = solve_hostname(ANY_ADDR);
struct ip_addr conf_listen         = conf_any_addr;
struct ip_addr conf_source         = conf_any_addr;
struct ip_addr *conf_src           = 0;

/* Funcoes Auxiliares */

int mask_len_value(const char *len)
{
  return atoi(len);
}

void set_protoname(proto_t proto)
{
  curr_proto = proto;
  ONVERBOSE(syslog(LOG_DEBUG, "Protocol: %s\n", get_protoname(curr_proto)))
}

int use_port(char *portname)
{
  int port = solve_port(portname, get_protoname(curr_proto));
  free(portname);
  return port;
}

struct ip_addr use_hostname(char *hostname)
{
  struct ip_addr ip = solve_hostname(hostname);
  free(hostname);
  return ip;
}

net_portion *use_hostprefix(char *hostname, int prefix_len)
{
  return new net_portion(use_hostname(hostname), prefix_len);  
}

to_addr *use_dstaddr(char *hostname, int port)
{
  return new dst_addr(on_the_fly_dns ? safe_strdup(hostname) : 0, 
		     use_hostname(hostname), 
		     port);
}


#line 172 "yconf.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yconf.h".  */
#ifndef YY_YY_YCONF_H_INCLUDED
# define YY_YY_YCONF_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TK_NAME = 258,
    TK_TCP = 259,
    TK_UDP = 260,
    TK_COLON = 261,
    TK_SCOLON = 262,
    TK_COMMA = 263,
    TK_SLASH = 264,
    TK_RANGE = 265,
    TK_LBRACE = 266,
    TK_RBRACE = 267,
    TK_ARROW = 268,
    TK_ACTV = 269,
    TK_PASV = 270,
    TK_USER = 271,
    TK_GROUP = 272,
    TK_BIND = 273,
    TK_LISTEN = 274,
    TK_SOURCE = 275,
    TK_XOR_KEY = 276,
    TK_REMOTE_SERVER = 277,
    TK_CONFUSING_KEY = 278,
    TK_STRING = 279,
    TK_FRAGILE = 280,
    TK_ILLEGAL = 281
  };
#endif
/* Tokens.  */
#define TK_NAME 258
#define TK_TCP 259
#define TK_UDP 260
#define TK_COLON 261
#define TK_SCOLON 262
#define TK_COMMA 263
#define TK_SLASH 264
#define TK_RANGE 265
#define TK_LBRACE 266
#define TK_RBRACE 267
#define TK_ARROW 268
#define TK_ACTV 269
#define TK_PASV 270
#define TK_USER 271
#define TK_GROUP 272
#define TK_BIND 273
#define TK_LISTEN 274
#define TK_SOURCE 275
#define TK_XOR_KEY 276
#define TK_REMOTE_SERVER 277
#define TK_CONFUSING_KEY 278
#define TK_STRING 279
#define TK_FRAGILE 280
#define TK_ILLEGAL 281

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 141 "conf.y" /* yacc.c:355  */

	int	           int_type;
  	bool               bool_type;
        char               *str_type;
        port_pair          *port_type;
        net_portion        *net_type;
	from_addr   	   *from_type;
	vector<from_addr*> *from_list_type;
	to_addr   	   *dst_type;
	vector<to_addr*>   *dst_list_type;
	host_map           *host_map_type;
	vector<host_map*>  *host_list_type;
	vector<int>        *port_list_type;
	proto_map          *map_type;
	vector<proto_map*> *map_list_type;
	entry		   *entry_type;

#line 282 "yconf.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YCONF_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 176 "conf.y" /* yacc.c:358  */

  /* Simbolo nao-terminal inicial */

#line 302 "yconf.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
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
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  26
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   86

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  27
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  52
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  96

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   281

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   184,   184,   185,   187,   188,   190,   191,   193,   194,
     195,   196,   197,   198,   199,   203,   205,   206,   208,   209,
     211,   212,   214,   216,   221,   226,   229,   233,   237,   242,
     248,   250,   255,   260,   265,   270,   274,   279,   284,   288,
     292,   297,   302,   305,   308,   311,   315,   320,   321,   323,
     327,   331,   335
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "TK_NAME", "TK_TCP", "TK_UDP",
  "TK_COLON", "TK_SCOLON", "TK_COMMA", "TK_SLASH", "TK_RANGE", "TK_LBRACE",
  "TK_RBRACE", "TK_ARROW", "TK_ACTV", "TK_PASV", "TK_USER", "TK_GROUP",
  "TK_BIND", "TK_LISTEN", "TK_SOURCE", "TK_XOR_KEY", "TK_REMOTE_SERVER",
  "TK_CONFUSING_KEY", "TK_STRING", "TK_FRAGILE", "TK_ILLEGAL", "$accept",
  "conf", "stmt_list", "stmt", "global_option", "entry", "fragile",
  "set_proto_tcp", "set_proto_udp", "section", "map_list", "map", "name",
  "port_list", "host_list", "host_map", "dst_list", "dst", "from_list",
  "from", "host_prefix", "prefix_length", "port_range", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281
};
# endif

#define YYPACT_NINF -65

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-65)))

#define YYTABLE_NINF -19

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      26,   -65,    13,    17,    19,    29,    35,    37,    59,    60,
     -65,    66,    26,   -65,   -65,   -65,    63,    39,   -65,   -65,
     -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,    65,
     -65,    39,   -65,    -5,   -65,   -65,     4,   -65,    65,   -65,
      65,    30,    65,    65,   -65,   -65,    18,    61,    -2,   -65,
      16,   -65,    67,    -7,    50,    65,    62,   -65,    68,   -65,
      30,   -65,    30,     3,    18,    30,    65,    30,    65,   -65,
      65,   -65,   -65,   -65,   -65,    69,    70,   -65,   -65,    45,
      58,    46,    71,   -65,    65,     3,   -65,    30,   -65,    30,
     -65,   -65,    47,    53,   -65,   -65
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,    21,     0,     0,     0,     0,     0,     0,     0,     0,
      19,     0,     3,     4,     7,     6,     0,     0,     8,     9,
      15,    10,    14,    11,    13,    12,     1,     5,    20,     0,
      17,     0,    30,     0,    23,    31,     0,    16,     0,    22,
       0,    42,     0,     0,    24,    32,     0,    47,     0,    33,
       0,    40,    43,     0,     0,     0,    49,    44,     0,    46,
      42,    25,    42,     0,     0,    42,     0,    42,     0,    51,
      50,    48,    34,    41,    39,     0,    35,    36,    45,     0,
       0,     0,     0,    52,     0,     0,    26,    42,    27,    42,
      38,    37,     0,     0,    28,    29
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -65,   -65,   -65,    64,   -65,   -65,   -65,   -65,   -65,    43,
     -65,    41,   -29,   -65,   -64,    20,   -65,    -8,   -65,    21,
     -65,   -65,    22
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    11,    12,    13,    14,    15,    16,    31,    17,    30,
      33,    34,    47,    36,    48,    49,    76,    77,    50,    51,
      52,    59,    57
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      35,    79,    38,    81,    65,    60,    32,    39,    66,    35,
      61,    45,    40,    53,    54,    41,    18,    56,    42,    43,
      19,    32,    20,    92,    62,    93,    69,    74,    55,    63,
     -18,     1,    21,    32,    75,    56,    46,    80,    22,    82,
      23,    83,     2,     3,     4,     5,     6,     7,     8,     9,
      29,    10,    60,    60,    60,    90,    75,    86,    88,    94,
      60,    67,    24,    25,    68,    95,    26,    28,    32,    87,
      58,    71,    70,    64,    37,    84,    27,    91,    85,    44,
      72,     0,    89,    73,     0,     0,    78
};

static const yytype_int8 yycheck[] =
{
      29,    65,     7,    67,    11,     7,     3,    12,    15,    38,
      12,    40,     8,    42,    43,    11,     3,    46,    14,    15,
       3,     3,     3,    87,     8,    89,    55,    24,    10,    13,
       4,     5,     3,     3,    63,    64,     6,    66,     3,    68,
       3,    70,    16,    17,    18,    19,    20,    21,    22,    23,
      11,    25,     7,     7,     7,    84,    85,    12,    12,    12,
       7,    11,     3,     3,    14,    12,     0,     4,     3,    11,
       9,     3,    10,     6,    31,     6,    12,    85,     8,    38,
      60,    -1,    11,    62,    -1,    -1,    64
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,    16,    17,    18,    19,    20,    21,    22,    23,
      25,    28,    29,    30,    31,    32,    33,    35,     3,     3,
       3,     3,     3,     3,     3,     3,     0,    30,     4,    11,
      36,    34,     3,    37,    38,    39,    40,    36,     7,    12,
       8,    11,    14,    15,    38,    39,     6,    39,    41,    42,
      45,    46,    47,    39,    39,    10,    39,    49,     9,    48,
       7,    12,     8,    13,     6,    11,    15,    11,    14,    39,
      10,     3,    42,    46,    24,    39,    43,    44,    49,    41,
      39,    41,    39,    39,     6,     8,    12,    11,    12,    11,
      39,    44,    41,    41,    12,    12
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    27,    28,    28,    29,    29,    30,    30,    31,    31,
      31,    31,    31,    31,    31,    31,    32,    32,    33,    33,
      34,    35,    36,    37,    37,    38,    38,    38,    38,    38,
      39,    40,    40,    41,    41,    42,    43,    43,    44,    44,
      45,    45,    46,    46,    46,    46,    47,    48,    48,    49,
      49,    49,    49
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     1,     1,     2,     1,     1,     2,     2,
       2,     2,     2,     2,     2,     2,     4,     3,     0,     1,
       0,     0,     3,     1,     3,     4,     6,     6,     8,     8,
       1,     1,     3,     1,     3,     3,     1,     3,     3,     1,
       1,     3,     0,     1,     2,     3,     2,     0,     2,     1,
       2,     2,     3
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
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
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
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

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

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

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
      yychar = yylex ();
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
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 6:
#line 190 "conf.y" /* yacc.c:1646  */
    { entry_vector-> push((yyvsp[0].entry_type)); }
#line 1441 "yconf.c" /* yacc.c:1646  */
    break;

  case 8:
#line 193 "conf.y" /* yacc.c:1646  */
    { conf_user = solve_user(conf_ident); }
#line 1447 "yconf.c" /* yacc.c:1646  */
    break;

  case 9:
#line 194 "conf.y" /* yacc.c:1646  */
    { conf_group = solve_group(conf_ident); }
#line 1453 "yconf.c" /* yacc.c:1646  */
    break;

  case 10:
#line 195 "conf.y" /* yacc.c:1646  */
    { conf_listen = solve_hostname(conf_ident); }
#line 1459 "yconf.c" /* yacc.c:1646  */
    break;

  case 11:
#line 196 "conf.y" /* yacc.c:1646  */
    { conf_xor_key = atoll(conf_ident); }
#line 1465 "yconf.c" /* yacc.c:1646  */
    break;

  case 12:
#line 197 "conf.y" /* yacc.c:1646  */
    { conf_confusing_key = atoll(conf_ident); }
#line 1471 "yconf.c" /* yacc.c:1646  */
    break;

  case 13:
#line 198 "conf.y" /* yacc.c:1646  */
    { conf_is_remote_server = conf_ident[0]=='Y' ||  conf_ident[0]=='y';}
#line 1477 "yconf.c" /* yacc.c:1646  */
    break;

  case 14:
#line 199 "conf.y" /* yacc.c:1646  */
    {
					conf_source = solve_hostname(conf_ident); 
					conf_src = &conf_source;
		}
#line 1486 "yconf.c" /* yacc.c:1646  */
    break;

  case 15:
#line 203 "conf.y" /* yacc.c:1646  */
    { conf_listen = solve_hostname(conf_ident); }
#line 1492 "yconf.c" /* yacc.c:1646  */
    break;

  case 16:
#line 205 "conf.y" /* yacc.c:1646  */
    { (yyval.entry_type) = new entry(P_TCP, (yyvsp[0].map_list_type), (yyvsp[-3].bool_type)); }
#line 1498 "yconf.c" /* yacc.c:1646  */
    break;

  case 17:
#line 206 "conf.y" /* yacc.c:1646  */
    { (yyval.entry_type) = new entry(P_UDP, (yyvsp[0].map_list_type), 0 /* false */); }
#line 1504 "yconf.c" /* yacc.c:1646  */
    break;

  case 18:
#line 208 "conf.y" /* yacc.c:1646  */
    { (yyval.bool_type) = 0; /* false */ }
#line 1510 "yconf.c" /* yacc.c:1646  */
    break;

  case 19:
#line 209 "conf.y" /* yacc.c:1646  */
    { (yyval.bool_type) = 1; /* true */ }
#line 1516 "yconf.c" /* yacc.c:1646  */
    break;

  case 20:
#line 211 "conf.y" /* yacc.c:1646  */
    { set_protoname(P_TCP); }
#line 1522 "yconf.c" /* yacc.c:1646  */
    break;

  case 21:
#line 212 "conf.y" /* yacc.c:1646  */
    { set_protoname(P_UDP); }
#line 1528 "yconf.c" /* yacc.c:1646  */
    break;

  case 22:
#line 214 "conf.y" /* yacc.c:1646  */
    { (yyval.map_list_type) = (yyvsp[-1].map_list_type); }
#line 1534 "yconf.c" /* yacc.c:1646  */
    break;

  case 23:
#line 216 "conf.y" /* yacc.c:1646  */
    {
			map_vector = new vector<proto_map*>();
			map_vector->push((yyvsp[0].map_type));
			(yyval.map_list_type) = map_vector;
		}
#line 1544 "yconf.c" /* yacc.c:1646  */
    break;

  case 24:
#line 221 "conf.y" /* yacc.c:1646  */
    {
			map_vector->push((yyvsp[0].map_type));
			(yyval.map_list_type) = map_vector;
		}
#line 1553 "yconf.c" /* yacc.c:1646  */
    break;

  case 25:
#line 226 "conf.y" /* yacc.c:1646  */
    {
			(yyval.map_type) = new proto_map((yyvsp[-3].port_list_type), (yyvsp[-1].host_list_type), 0, 0, conf_user, conf_group, conf_listen, conf_src, conf_xor_key, conf_confusing_key, conf_is_remote_server);
		}
#line 1561 "yconf.c" /* yacc.c:1646  */
    break;

  case 26:
#line 229 "conf.y" /* yacc.c:1646  */
    {
		        struct ip_addr ip = use_hostname((yyvsp[-3].str_type));
			(yyval.map_type) = new proto_map((yyvsp[-5].port_list_type), (yyvsp[-1].host_list_type), &ip, 0, conf_user, conf_group, conf_listen, conf_src, conf_xor_key, conf_confusing_key, conf_is_remote_server);
		}
#line 1570 "yconf.c" /* yacc.c:1646  */
    break;

  case 27:
#line 233 "conf.y" /* yacc.c:1646  */
    {
		        struct ip_addr ip = use_hostname((yyvsp[-3].str_type));
			(yyval.map_type) = new proto_map((yyvsp[-5].port_list_type), (yyvsp[-1].host_list_type), 0, &ip, conf_user, conf_group, conf_listen, conf_src, conf_xor_key, conf_confusing_key, conf_is_remote_server);
		}
#line 1579 "yconf.c" /* yacc.c:1646  */
    break;

  case 28:
#line 237 "conf.y" /* yacc.c:1646  */
    {
		        struct ip_addr ip1 = use_hostname((yyvsp[-5].str_type));
		        struct ip_addr ip2 = use_hostname((yyvsp[-3].str_type));
			(yyval.map_type) = new proto_map((yyvsp[-7].port_list_type), (yyvsp[-1].host_list_type), &ip1, &ip2, conf_user, conf_group, conf_listen, conf_src, conf_xor_key, conf_confusing_key, conf_is_remote_server);
		}
#line 1589 "yconf.c" /* yacc.c:1646  */
    break;

  case 29:
#line 242 "conf.y" /* yacc.c:1646  */
    {
		        struct ip_addr ip1 = use_hostname((yyvsp[-5].str_type));
		        struct ip_addr ip2 = use_hostname((yyvsp[-3].str_type));
			(yyval.map_type) = new proto_map((yyvsp[-7].port_list_type), (yyvsp[-1].host_list_type), &ip2, &ip1, conf_user, conf_group, conf_listen, conf_src, conf_xor_key, conf_confusing_key, conf_is_remote_server);
		}
#line 1599 "yconf.c" /* yacc.c:1646  */
    break;

  case 30:
#line 248 "conf.y" /* yacc.c:1646  */
    { (yyval.str_type) = safe_strdup(conf_ident); }
#line 1605 "yconf.c" /* yacc.c:1646  */
    break;

  case 31:
#line 250 "conf.y" /* yacc.c:1646  */
    {
			port_vector = new vector<int>();
			port_vector->push(use_port((yyvsp[0].str_type)));
			(yyval.port_list_type) = port_vector; 
		}
#line 1615 "yconf.c" /* yacc.c:1646  */
    break;

  case 32:
#line 255 "conf.y" /* yacc.c:1646  */
    {
			port_vector->push(use_port((yyvsp[0].str_type)));
			(yyval.port_list_type) = port_vector; 
		}
#line 1624 "yconf.c" /* yacc.c:1646  */
    break;

  case 33:
#line 260 "conf.y" /* yacc.c:1646  */
    {
              		host_vector = new vector<host_map*>();
			host_vector->push((yyvsp[0].host_map_type));
			(yyval.host_list_type) = host_vector;
		}
#line 1634 "yconf.c" /* yacc.c:1646  */
    break;

  case 34:
#line 265 "conf.y" /* yacc.c:1646  */
    {
			host_vector->push((yyvsp[0].host_map_type));
			(yyval.host_list_type) = host_vector;
		}
#line 1643 "yconf.c" /* yacc.c:1646  */
    break;

  case 35:
#line 270 "conf.y" /* yacc.c:1646  */
    {
			(yyval.host_map_type) = new host_map((yyvsp[-2].from_list_type), (yyvsp[0].dst_list_type));
		}
#line 1651 "yconf.c" /* yacc.c:1646  */
    break;

  case 36:
#line 274 "conf.y" /* yacc.c:1646  */
    {
			dst_vector = new vector<to_addr*>();
			dst_vector->push((yyvsp[0].dst_type));
			(yyval.dst_list_type) = dst_vector;
                }
#line 1661 "yconf.c" /* yacc.c:1646  */
    break;

  case 37:
#line 279 "conf.y" /* yacc.c:1646  */
    {
			dst_vector->push((yyvsp[0].dst_type));
			(yyval.dst_list_type) = dst_vector;
                }
#line 1670 "yconf.c" /* yacc.c:1646  */
    break;

  case 38:
#line 284 "conf.y" /* yacc.c:1646  */
    {
			int port = use_port((yyvsp[0].str_type));   /* solve portname */
			(yyval.dst_type) = use_dstaddr((yyvsp[-2].str_type), port); /* new dst_addr() */
                }
#line 1679 "yconf.c" /* yacc.c:1646  */
    break;

  case 39:
#line 288 "conf.y" /* yacc.c:1646  */
    {
                        (yyval.dst_type) = new director(conf_lex_str_buf);
		}
#line 1687 "yconf.c" /* yacc.c:1646  */
    break;

  case 40:
#line 292 "conf.y" /* yacc.c:1646  */
    {
			from_vector = new vector<from_addr*>();
			from_vector->push((yyvsp[0].from_type));
			(yyval.from_list_type) = from_vector;
		}
#line 1697 "yconf.c" /* yacc.c:1646  */
    break;

  case 41:
#line 297 "conf.y" /* yacc.c:1646  */
    {
			from_vector->push((yyvsp[0].from_type));
			(yyval.from_list_type) = from_vector;
		}
#line 1706 "yconf.c" /* yacc.c:1646  */
    break;

  case 42:
#line 302 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.from_type) = new from_addr(new net_portion(solve_hostname(ANY_ADDR), MIN_MASK_LEN), new port_pair(FIRST_PORT, LAST_PORT)); 
		}
#line 1714 "yconf.c" /* yacc.c:1646  */
    break;

  case 43:
#line 305 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.from_type) = new from_addr((yyvsp[0].net_type), new port_pair(FIRST_PORT, LAST_PORT)); 
		}
#line 1722 "yconf.c" /* yacc.c:1646  */
    break;

  case 44:
#line 308 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.from_type) = new from_addr(new net_portion(solve_hostname(ANY_ADDR), MIN_MASK_LEN), (yyvsp[0].port_type)); 
		}
#line 1730 "yconf.c" /* yacc.c:1646  */
    break;

  case 45:
#line 311 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.from_type) = new from_addr((yyvsp[-2].net_type), (yyvsp[0].port_type)); 
		}
#line 1738 "yconf.c" /* yacc.c:1646  */
    break;

  case 46:
#line 315 "conf.y" /* yacc.c:1646  */
    { 
			/* use_hostprefix(): new net_portion() */
  			(yyval.net_type) = use_hostprefix((yyvsp[-1].str_type), (yyvsp[0].int_type)); 
		}
#line 1747 "yconf.c" /* yacc.c:1646  */
    break;

  case 47:
#line 320 "conf.y" /* yacc.c:1646  */
    { (yyval.int_type) = MAX_MASK_LEN; }
#line 1753 "yconf.c" /* yacc.c:1646  */
    break;

  case 48:
#line 321 "conf.y" /* yacc.c:1646  */
    { (yyval.int_type) = mask_len_value(conf_ident); }
#line 1759 "yconf.c" /* yacc.c:1646  */
    break;

  case 49:
#line 323 "conf.y" /* yacc.c:1646  */
    {
			int port = use_port((yyvsp[0].str_type));
			(yyval.port_type) = new port_pair(port, port);
		}
#line 1768 "yconf.c" /* yacc.c:1646  */
    break;

  case 50:
#line 327 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.port_type) = new port_pair(use_port((yyvsp[-1].str_type)), 
					   LAST_PORT); 
		}
#line 1777 "yconf.c" /* yacc.c:1646  */
    break;

  case 51:
#line 331 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.port_type) = new port_pair(FIRST_PORT, 
				           use_port((yyvsp[0].str_type))); 
		}
#line 1786 "yconf.c" /* yacc.c:1646  */
    break;

  case 52:
#line 335 "conf.y" /* yacc.c:1646  */
    { 
			(yyval.port_type) = new port_pair(use_port((yyvsp[-2].str_type)),
				           use_port((yyvsp[0].str_type)));
		}
#line 1795 "yconf.c" /* yacc.c:1646  */
    break;


#line 1799 "yconf.c" /* yacc.c:1646  */
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

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
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
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 341 "conf.y" /* yacc.c:1906  */


/* C code */

/* eof: conf.y */

