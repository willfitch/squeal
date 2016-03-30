#include <check.h>
#include <squeal.h>
#include <squeal_function.h>

START_TEST(test_function_init)
{
    SqlFunction *function = sql_function_init();
    ck_assert_ptr_eq(function->name, NULL);
    ck_assert_int_eq(0, function->total_params);
    ck_assert_int_eq(0, function->used_params);
    sql_function_free(function);
} END_TEST

START_TEST(test_function_param_init)
{
    Parameter *parameter = parameter_init();
    ck_assert_ptr_eq(parameter->name, NULL);
    sql_parameter_free(parameter);
} END_TEST

START_TEST(test_sql_function_param_init)
{
    SqlFunction *function = sql_function_param_init(2);
    ck_assert_int_eq(2, function->total_params);
    ck_assert_ptr_eq(function->name, NULL);
    ck_assert_int_eq(2, function->total_params);
    ck_assert_int_eq(0, function->used_params);
    sql_function_free(function);
} END_TEST

START_TEST(test_sql_function_add_param)
{
    SqlFunction *function = sql_function_init();
    Parameter *parameter = parameter_init();
    SquealString *str = squeal_string_init("will", sizeof("will"));

    parameter->name = str;

    sql_function_add_param(&function, parameter);

    ck_assert_int_eq(1, function->total_params);
    ck_assert_int_eq(1, function->used_params);
    ck_assert_str_eq(parameter->name->val, function->params[0]->name->val);
    ck_assert_ptr_eq(parameter, function->params[0]);
    sql_function_free(function);
} END_TEST
