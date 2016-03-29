#ifndef HAVE_SQUEAL_AST_H
#define HAVE_SQUEAL_AST_H

#include <squeal.h>
#include <stdarg.h>

#define SQUEAL_AST_CHILD_SHIFT_ZERO 6
#define SQUEAL_AST_CHILD_SHIFT 8

enum _squeal_ast_kind {
    /* 0 params */
    SQUEAL_AST_USE = 1 << SQUEAL_AST_CHILD_SHIFT,

    /* 1 param */
    SQUEAL_AST_SHOW_DATABASES = 1 << SQUEAL_AST_CHILD_SHIFT_ZERO,
    SQUEAL_AST_NAME_LIST,

    SQUEAL_AST_ORDER_BY,
    SQUEAL_AST_SELECT,
    SQUEAL_AST_FUNCTION,
    SQUEAL_AST_TABLE,
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

typedef struct _squeal_ast squeal_ast;
typedef uint32_t squeal_ast_kind;
typedef uint32_t squeal_ast_attr;

struct _squeal_ast {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    uint32_t lineno;
    squeal_ast *child[1];
};

typedef struct _squeal_ast_list {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    uint32_t lineno;
    uint32_t children;
    squeal_ast *child[1];
} squeal_ast_list;

typedef struct _squeal_ast_sval {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    squeal_val val;
} squeal_ast_sval;

typedef struct _squeal_ast_func {
    squeal_ast_kind kind;
    squeal_ast_attr attr;
    uint32_t lineno;
    squeal_function *function;
} squeal_ast_func;

squeal_ast *squeal_ast_create(uint32_t kind, ...);
squeal_ast *squeal_ast_create_list(uint32_t children, squeal_ast_kind kind, ...);
squeal_ast *squeal_ast_list_add(squeal_ast *ast, squeal_ast *op);
void squeal_ast_free(squeal_ast *ast);
#endif