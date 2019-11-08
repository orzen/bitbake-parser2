/* A Bison parser, made by GNU Bison 3.4.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_TAB_H_INCLUDED
# define YY_YY_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 16 "parser.y"

#include <glib.h>
#include "lexer.h"

#line 53 "parser.tab.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FAKEROOT = 258,
    PYTHON = 259,
    EXPORT = 260,
    PREDOT_ASSIGN = 261,
    POSTDOT_ASSIGN = 262,
    PREPEND_ASSIGN = 263,
    APPEND_ASSIGN = 264,
    COLON_ASSIGN = 265,
    QUES_ASSIGN = 266,
    LAZYQUES_ASSIGN = 267,
    ASSIGN = 268,
    ADDTASK = 269,
    ADDTASK_AFTER = 270,
    ADDTASK_BEFORE = 271,
    UNSET = 272,
    ADDTASK_CONTENT = 273,
    BLOCK_START = 274,
    BLOCK_CONTENT = 275,
    SLI_CONTENT = 276,
    MLI_CONTENT = 277,
    WORD = 278,
    STRING = 279,
    STRING_CONTINUATION = 280,
    SINGLE_QUOTE = 281,
    DOUBLE_QUOTE = 282,
    INHERIT = 283,
    INCLUDE = 284,
    REQUIRE = 285,
    DELTASK = 286,
    EXPORT_FUNCTIONS = 287
  };
#endif

/* Value type.  */



int yyparse (yyscan_t yyscanner, GNode **acc);

#endif /* !YY_YY_PARSER_TAB_H_INCLUDED  */
