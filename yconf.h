/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
#line 141 "conf.y" /* yacc.c:1909  */

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

#line 124 "yconf.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_YCONF_H_INCLUDED  */
