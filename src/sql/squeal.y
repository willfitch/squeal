%{
#include "squeal_ast.h"

%}

%pure_parser

%destructor { squeal_ast_destroy($$); } <ast>
%destructor { if ($$) squeal_string_free($$); } <str>

%left OR
%left AND
%left NOT
%left '+' '-'
%left '*' '/'

%token <ast> INTEGER "integer"
%token <ast> FLOAT "float"
%token <ast> STRING "identifier"

%token END 0 "end of statement"
%token QUALIFIED_IDENTIFIER "identifier"
%token QUALIFIED_STAR "asterisk"
%token SELECT "SELECT"
%token USE "USE"
%token INDEX "INDEX"
%token TOP "TOP"
%token SHOW "SHOW"
%token DATABASES "DATABASES"
%token FROM "FROM"
%token WHERE "WHERE"
%token BETWEEN "BETWEEN"
%token GROUP_BY "GROUP BY"
%token HAVING "HAVING"
%token ORDER_BY "ORDER BY"
%token SET_OPERATOR "SET_OPERATOR"
%token NAMED_PARAM_SEP "NAMED_PARAM_SEP"
%token COMMA ", COMMA"
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
%token LPAREN "( LPAREN"
%token RPAREN ") RPAREN"
%token CONCAT "|| CONCAT"
%token LCURLY "{ LCURLY"
%token RCURLY "} RCURLY"
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
%token INNER_JOIN "INNER JOIN"
%token LEFT_OUTER_JOIN "LEFT OUTER JOIN"
%token RIGHT_OUTER_JOIN "RIGHT OUTER JOIN"
%token JOIN "JOIN"
%token LEFT_JOIN "LEFT JOIN"
%token RIGHT_JOIN "RIGHT JOIN"
%token FULL_JOIN "FULL JOIN"
%token NATURAL_JOIN "NATURAL JOIN"
%token CROSS_JOIN "CROSS JOIN"
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
%token <ival> INTEGER "INTEGER"
%token <strval> IDENTIFIER "IDENTIFIER"
%token <strval> QUOTED_IDENTIFIER "QUOTED_IDENTIFIER"

/* force parse error */
%token T_ERROR

%start sql_query

%%

sql_query:
    sql_statement { $$ = squeal_ast_create(); }
;

identifier:
    STRING { $$ = $1; }
;



%%