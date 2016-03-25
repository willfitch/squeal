#ifndef HAVE_SQUEAL_AST_H
#define HAVE_SQUEAL_AST_H

#include <stdlib.h>
#include <squeal_types.h>

typedef struct _squeal_ast squeal_ast;
typedef uint16_t squeal_ast_kind;
typedef uint16_t squeal_ast_attr;
typedef struct _squeal_ast_list squeal_ast_list;
typedef struct _squeal_ast_val squeal_ast_val;

enum _squeal_ast_kind {
    SQUEAL_AST_USE,
    SQUEAL_AST_SHOW_DATABASES,
    SQUEAL_AST_SELECT,
    SQUEAL_AST_FUNCTION,
    SQUEAL_AST_TABLE,
    SQUEAL_AST_SET_OPERATOR,
    SQUEAL_AST_AND,
    SQUEAL_AST_OR,
    SQUEAL_AST_CONDITION,
    SQUEAL_AST_TERM,
    SQUEAL_AST_BINARY_CONDITION,
    SQUEAL_AST_EXISTS_CONDITION,
    SQUEAL_AST_NOT_CONDITION,
    SQUEAL_AST_COMPARE,
    SQUEAL_AST_IS_NULL,
    SQUEAL_AST_IS_NOT_NULL,
    SQUEAL_AST_IS_TRUE,
    SQUEAL_AST_IS_FALSE,
    SQUEAL_AST_IN,
    SQUEAL_AST_NOT_IN,
    SQUEAL_AST_LIKE,
    SQUEAL_AST_BETWEEN,
    SQUEAL_AST_NOT_BETWEEN,
    SQUEAL_AST_OPERAND,
    SQUEAL_AST_SUMMAND,
    SQUEAL_AST_FACTOR,
    SQUEAL_AST_CAST,
    SQUEAL_AST_CASE,
    SQUEAL_AST_CASE_ITEM
};

struct _squeal_ast {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    uint32_t lineno;
    squeal_ast *child[1];
};

struct _squeal_ast_list {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    uint32_t lineno;
    uint32_t children;
    squeal_ast *child[1];
};

struct _squeal_ast_val {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    squeal_val val;
};

struct _squeal_ast_func {

};


#endif