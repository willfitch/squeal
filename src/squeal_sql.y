
%token_type {Token}
%default_type {Token}

%extra_argument {SqlStatement *statement}

%syntax_error {
    fprintf(stderr, "syntax error");
}

%stack_overflow {
    fprintf(stderr, "stack overflow");
}

%name squealParser

%include {
#include <squeal.h>
#include <squeal_hashtable.h>
#include <squeal_function.h>
#include "squeal_compile.h"

/*
** Disable all error recovery processing in the parser push-down
** automaton.
*/
#define YYNOERRORRECOVERY 1

/*
** Indicate that sqlite3ParserFree() will never be called with a null
** pointer.
*/
#define YYPARSEFREENEVERNULL 1
}

// SQL input
input ::= cmdlist.
cmdlist ::= cmdlist ecmd.
cmdlist ::= ecmd.
ecmd ::= SEMICOLON.
ecmd ::= cmdx SEMICOLON.
cmdx ::= cmd.

%type select {Select*}
%destructor select {sql_select_free($$);}


// SELECT statement
cmd ::= select(X) {

}

select(A) ::= SELECT(S) column_list(C) from(F) where(W) group_by(G) order_by(O) limit(L) offset(O). {
    Select *select = squeal_compile_select(statement, C, F, W, G, O, L, O);
    A = select;
}