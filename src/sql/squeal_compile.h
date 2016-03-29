#ifndef HAVE_SQUEAL_COMPILE_H
#define HAVE_SQUEAL_COMPILE_H

#include <squeal.h>
#include "squeal_ast.h"

struct _squeal_function_param {
    squeal_string *name;
};

struct _squeal_function {
    squeal_string *name;
    hashtable *params;
};

void squeal_function_free(squeal_function *func);
void squeal_function_param_free(squeal_function_param *param);

#endif