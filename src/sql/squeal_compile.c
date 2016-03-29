#include "squeal_compile.h"



void squeal_function_param_free(squeal_function_param *param)
{
    if (param) {
        squeal_string_free(param->name);
    }
}

void squeal_function_free(squeal_function *func)
{
    squeal_string *key;
    squeal_function_param *param;

    if (func->name) {
        squeal_string_free(func->name);
    }

    SQUEAL_HASHTABLE_ITERATE_PTR(func->params, key, param)
    {
        squeal_function_param_free(param);
    } SQUEAL_HASH_ITERATE_END();

    squeal_ht_free(func->params);
}