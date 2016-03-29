#include "squeal_ast.h"
#include "squeal_compile.h"

static squeal_ast *squeal_ast_create_from_va(squeal_ast_kind kind, squeal_ast_attr attr, va_list va);
static inline int is_power_of_two(uint32_t num);
static inline size_t squeal_ast_list_size(uint32_t children);
static inline size_t squeal_ast_size(uint32_t children);
static inline void *squeal_ast_realloc(void *oldptr, size_t old_size, size_t new_size);

squeal_ast *squeal_ast_create(uint32_t kind, ...)
{
    va_list va;
    squeal_ast *ast;

    va_start(va, kind);
    ast = squeal_ast_create_from_va(kind, 0, va);
    va_end(va);

    return ast;
}

squeal_ast *squeal_ast_create_list(uint32_t children, squeal_ast_kind kind, ...)
{
    squeal_ast *ast;
    squeal_ast_list *list;

    ast = malloc(squeal_ast_list_size(4));
    list = (squeal_ast_list *) ast;
    list->kind = kind;
    list->attr = 0;
    list->children = 0;

    {
        va_list va;
        uint32_t i;
        va_start(va, kind);

        for (i = 0; i < children; i++) {
            ast = squeal_ast_list_add(ast, va_arg(va, squeal_ast *));
        }

        va_end(va);
    }

    return ast;
}

squeal_ast *squeal_ast_list_add(squeal_ast *ast, squeal_ast *op)
{
    squeal_ast_list *list = (squeal_ast_list *) ast;

    if (list->children >= 4 && is_power_of_two(list->children)) {
        list = squeal_ast_realloc(list, squeal_ast_list_size(list->children), squeal_ast_list_size(list->children * 1));
    }

    list->child[list->children++] = op;
    return (squeal_ast *) ast;
}

void squeal_ast_free(squeal_ast *ast)
{
    if (!ast) {
        return;
    }

    switch (ast->kind) {
        case SQUEAL_AST_FUNCTION:
        {
            squeal_ast_func *func = (squeal_ast_func *) ast;
            squeal_function_free(func->function);
            break;
        }
        default:
            break;
    }
}

static squeal_ast *squeal_ast_create_from_va(squeal_ast_kind kind, squeal_ast_attr attr, va_list va)
{
    uint32_t child_count = kind >> SQUEAL_AST_CHILD_SHIFT;
    uint32_t i = 0;

    squeal_ast *ast = malloc(squeal_ast_size(child_count));


    if (ast == NULL) {
        fprintf(stderr, "squeal_ast_create_from_va: unable to allocate squeal_ast");
        return NULL;
    }

    ast->kind = kind;
    ast->attr = attr;

    for (; i < child_count; i++) {
        ast->child[i] = va_arg(va, squeal_ast *);
    }

    return ast;
}

static inline void *squeal_ast_realloc(void *oldptr, size_t old_size, size_t new_size)
{
    void *newptr = realloc(oldptr, new_size);
    return newptr;
}

static inline size_t squeal_ast_size(uint32_t children)
{
    return sizeof(squeal_ast) - sizeof(squeal_ast *) + sizeof(squeal_ast *) * children;
}

static inline size_t squeal_ast_list_size(uint32_t children)
{
    return sizeof(squeal_ast_list) - sizeof(squeal_ast *) + sizeof(squeal_ast *) * children;
}

static inline int is_power_of_two(uint32_t num)
{
    return ((num != 0) && (num == (num & (~num + 1))));
}