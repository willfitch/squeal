#include <squeal_function.h>

static squeal_always_inline void clean_allocated_function(SqlFunction **function);

SqlFunction *sql_function_init()
{
    SqlFunction *function = malloc(sizeof(SqlFunction));

    if (function == NULL) {
        fprintf(stderr, "sql_function_init: unable to allocate function");
        return NULL;
    }

    clean_allocated_function(&function);

    return function;
}

SqlFunction *sql_function_param_init(uint16_t num_params)
{
    SqlFunction * function;
    function = malloc(sizeof(SqlFunction) + sizeof(Parameter) * num_params);

    if (function == NULL) {
        fprintf(stderr, "sql_function_param_init: unable to allocate function");
        return NULL;
    }

    clean_allocated_function(&function);
    function->total_params = num_params;

    return function;
}

static squeal_always_inline void clean_allocated_function(SqlFunction **function)
{
    (*function)->name = NULL;
    (*function)->total_params = 0;
    (*function)->used_params = 0;
}

Parameter *parameter_init()
{
    Parameter *param = (Parameter *) malloc(sizeof(Parameter));

    if (param == NULL) {
        fprintf(stderr, "parameter_init: unable to allocate parameter");
        return NULL;
    }

    param->name = NULL;
    param->v.parameter = NULL;
    param->v.sval = NULL;

    return param;
}

void sql_function_add_param(SqlFunction **func, Parameter *param)
{
    uint16_t previous_total_params = (*func)->total_params;
    uint16_t previous_used_params = (*func)->used_params;

    if ((*func)->total_params == (*func)->used_params) {
        sql_function_realloc(func, 1);
    }

    (*func)->total_params = previous_total_params;
    (*func)->used_params = previous_used_params;

    (*func)->params[previous_used_params] = param;
    (*func)->total_params++;
    (*func)->used_params++;
}

void sql_parameter_free(Parameter *param)
{
    if (param) {
        if (param->v.sval) {
            squeal_sval_free(param->v.sval);
        }

        if (param->v.parameter) {
            sql_parameter_free(param->v.parameter);
        }

        squeal_string_free(param->name);
        free(param);
    }
}

void sql_function_free(SqlFunction *func)
{
    if (func) {
        int i = 0;
        squeal_string_free(func->name);

        for (; i < func->used_params; i++) {
            sql_parameter_free(func->params[i]);
        }

        free(func);
    }
}
