#ifndef HAVE_SQUEAL_FUNCTION_H
#define HAVE_SQUEAL_FUNCTION_H

#include <squeal.h>

#define SQL_PARAMETER_IN (1 << 1)
#define SQL_PARAMETER_OUT (1 << 2)

struct _parameter {
    SquealString *name; /* name of the parameter */
    short is_named; /* 1 = named, 0 = not */
    short in_out; /* whether in or out */

    union {
        sval *sval;
        Parameter *parameter;
    } v; /* parameter value */
};

struct _sql_function {
    SquealString *name; /* name of the function */

    uint16_t total_params;
    uint16_t used_params;

    Parameter *params[1]; /* struct hack. must be last! */
};

SqlFunction *sql_function_init();
SqlFunction *sql_function_param_init(uint16_t num_params);
Parameter *parameter_init();
void sql_parameter_free(Parameter *param);
void sql_function_add_param(SqlFunction **func, Parameter *param);
void sql_function_free(SqlFunction *func);

static squeal_always_inline int sql_function_realloc(SqlFunction **func, size_t more_params)
{
    *func = (SqlFunction *) realloc(*func, sizeof(SqlFunction) + sizeof(Parameter) * more_params);

    if (*func == NULL) {
        fprintf(stderr, "sql_function_realloc: unable to realloc function");
        return -1;
    }

    (*func)->total_params += more_params;
    return 1;
}


#endif