%{
#include "squeal_compile.h"

enum _squeal_ast_kind exposed;

#define YYSIZE_T size_t
#define YYERROR_VERBOSE

%}

%pure_parser

%destructor { squeal_ast_destroy($$); } <ast>

%left OR
%left AND
%left NOT
%left '+' '-'
%left '*' '/'

%token INTEGER "integer"
%token FLOAT "float"
%token STRING "identifier"

%token END 0 "end of statement"
%token SELECT "SELECT"
%token USE "USE"
%token INDEX "INDEX"
%token TOP "TOP"
%token SHOW "SHOW"
%token DATABASES "DATABASES"
%token FROM "FROM"
%token WHERE "WHERE"
%token BETWEEN "BETWEEN"
%token GROUP "GROUP"
%token BY "BY"
%token HAVING "HAVING"
%token ORDER_BY "ORDER BY"
%token SET_OPERATOR "SET_OPERATOR"
%token NAMED_PARAM_SEP "NAMED_PARAM_SEP"
%token PLUS "+ PLUS"
%token MINUS "- MINUS"
%token DIVIDE "/ DIVIDE"
%token STAR "* STAR"
%token MODULO "% MODULO"
%token CMP_EQUALS "= EQUAL"
%token CMP_NOT_EQUALS "!= NOT_EQUAL"
%token CMP_NOT_EQUALS_BASIC "<> NOT_EQUAL"
%token CMP_GREATER_OR_EQUAL ">= GREATER_EQUAL"
%token CMP_GREATER "> GREATER"
%token CMP_LESS_OR_EQUAL "<= LESS_EQUAL"
%token CMP_LESS "< LESS"
%token CONCAT "|| CONCAT"
%token AS "AS"
%token ALL "ALL"
%token ANY "ANY"
%token SOME "SOME"
%token EXISTS "EXISTS"
%token IS "IS"
%token IN "IN"
%token ON "ON"
%token LOGICAL_AND "AND"
%token LOGICAL_OR "OR"
%token LOGICAL_NOT "NOT"
%token INNER "INNER"
%token OUTER "OUTER"
%token JOIN "JOIN"
%token LEFT "LEFT"
%token RIGHT "RIGHT"
%token FULL "FULL"
%token NATURAL "NATURAL"
%token CROSS "CROSS"
%token CASE "CASE"
%token WHEN "WHEN"
%token THEN "THEN"
%token ELSE "ELSE"
%token END "END"
%token IF "IF"
%token LIKE "LIKE"
%token ASC "ASC"
%token DESC "DESC"
%token NULLS "NULLS"
%token FIRST "FIRST"
%token LAST "LAST"
%token OPTION "OPTION"
%token WITH "WITH"
%token CONNECTION "CONNECTION"
%token CAST "CAST"
%token LIMIT "LIMIT"
%token OFFSET "OFFSET"
%token BACKEND "BACKEND"
%token NULL "NULL"
%token BOOLEAN "BOOLEAN"
%token BIND "BIND"
%token INTEGER "INTEGER"
%token IDENTIFIER "IDENTIFIER"
%token QUOTED_IDENTIFIER "QUOTED_IDENTIFIER"

/* force parse error */
%token T_ERROR

%type <ast> identifier sql_statement

%start sql_query

%%

sql_query:
    sql_statement { $$ = $1; }
;

identifier:
    STRING { $$ = $1; }
;

name:
    IDENTIFIER  { $$ = $1; }
;

orderby_statement:
        /* empty */ { $$ = NULL; }
    |   ORDER BY name_list      { $$ = squeal_ast_create(SQUEAL_AST_ORDER_BY, $2, NULL); }
    |   ORDER BY name_list ASC  { $$ = squeal_ast_create(SQUEAL_AST_ORDER_BY, $2, ORDER_BY_ASC)}
    |   ORDER BY name_list DESC { $$ = squeal_ast_create(SQUEAL_AST_ORDER_BY, $2, ORDER_BY_DESC)}
;

column_list:
        /* empty */ { $$ = NULL; }
    |   name_list { $$ = $1; }
;

name_list:
        name { $$ = squeal_ast_create_list(1, SQUEAL_AST_NAME_LIST, $1); }
    |   name_list ',' name { $$ = squeal_ast_list_add($1, $3); }
;


%%