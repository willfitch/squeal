%{
    int lineno = 1;
    void yyerror(char *);

%}

%x BLOCK_COMMENT
%x LINE_COMMENT

%s SQL

%%

"select"                                                { return SELECT; }
"use"                                                   { return USE; }
"index"                                                 { return INDEX; }
"top"                                                   { return TOP; }
"show"                                                  { return SHOW; }
"databases"                                             { return DATABASES; }
"from"                                                  { return FROM; }
"where"                                                 { return WHERE; }
"between"                                               { return BETWEEN; }
"group"                                                 { return GROUP; }
"by"                                                    { return BY; }
"having"                                                { return HAVING; }
"order"                                                 { return ORDER; }
":="                                                    { return NAMED_PARAM_SEP; }
"+"                                                     { return PLUS; }
"-"                                                     { return MINUS; }
"/"                                                     { return DIVIDE; }
"*"                                                     { return STAR; }
"%"                                                     { return MODULO; }
"="                                                     { return CMP_EQUALS; }
"!="                                                    { return CMP_NOT_EQUALS; }
"<>"                                                    { return CMP_NOT_EQUALS_BASIC; }
">="                                                    { return CMP_GREATER_OR_EQUAL; }
">"                                                     { return CMP_GREATER; }
"<="                                                    { return CMP_LESS_OR_EQUAL; }
"<"                                                     { return CMP_LESS; }
"||"                                                    { return CONCAT; }
"as"                                                    { return AS; }
"all"                                                   { return ALL; }
"any"                                                   { return ANY; }
"some"                                                  { return SOME; }
"exists"                                                { return EXISTS; }
"is"                                                    { return IS; }
"in"                                                    { return IN; }
"on"                                                    { return ON; }
"and"                                                   { return LOGICAL_AND; }
"or"                                                    { return LOGICAL_OR; }
"not"                                                   { return LOGICAL_NOT; }
"inner"                                                 { return INNER; }
"outer"                                                 { return OUTER; }
"join"                                                  { return JOIN; }
"left"                                                  { return LEFT; }
"right"                                                 { return RIGHT; }
"full"                                                  { return FULL; }
"natural"                                               { return NATURAL; }
"cross"                                                 { return CROSS; }
"case"                                                  { return CASE; }
"when"                                                  { return WHEN; }
"then"                                                  { return THEN; }
"else"                                                  { return ELSE; }
"end"                                                   { return END; }
"if"                                                    { return IF; }
"like"                                                  { return LIKE; }
"asc"                                                   { return ASC; }
"desc"                                                  { return DESC; }
"nulls"                                                 { return NULLS; }
"first"                                                 { return FIRST; }
"last"                                                  { return LAST; }
"option"                                                { return OPTION; }
"with"                                                  { return WITH; }
"connection"                                            { return CONNECTION; }
"cast"                                                  { return CAST; }
"limit"                                                 { return LIMIT; }
"offset"                                                { return OFFSET; }
"backend"                                               { return BACKEND; }
"null"                                                  { return NULL; }
"boolean"                                               { return BOOLEAN; }
"bind"                                                  { return BIND; }
[+-]?[0-9]+                                             { return INTEGER; }
([0-9]+|([0-9]*\.[0-9]+)([eE][-+]?[0-9]+)?)             { return DOUBLE; }

<BLOCK_COMMENT>\*\/                                     { BEGIN(INITIAL); }
<BLOCK_COMMENT>\n                                       { yylineno++; }
<BLOCK_COMMENT>.                                        { /* ignore */ }
"--"                                                    { BEGIN(LINE_COMMENT); }
<LINE_COMMENT>\n                                        { BEGIN(INITIAL); yylineno++; }
<LINE_COMMENT>.                                         { /* ignore */ }
[a-zA-Z][a-zA-Z0-9_]*                                   { return IDENTIFIER; }
((\"[^\"]*\")|(\'[^\']*\'))                             { return STRING; }
[ \t\r]+                                                { /* ignore */ }
\n                                                      { yylineno++; }

%%

