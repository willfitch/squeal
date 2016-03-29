#ifndef HAVE_SQUEAL_COMPILE_H
#define HAVE_SQUEAL_COMPILE_H

#include <squeal.h>

struct _squeal_function_param {
    SquealString *name;
};

struct _squeal_function {
    SquealString *name;
    HashTable *params;
};

void squeal_function_free(squeal_function *func);
void squeal_function_param_free(squeal_function_param *param);

#endif