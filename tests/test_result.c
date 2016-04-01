#include <check.h>
#include <squeal.h>
#include <squeal_result.h>

START_TEST(test_sql_result_column_init)
{
    ResultColumn *column = sql_result_column_init();
    ck_assert_ptr_eq(column->next, NULL);
    ck_assert_ptr_eq(column->catalog, NULL);
    ck_assert_ptr_eq(column->table, NULL);
    ck_assert_ptr_eq(column->orig_table, NULL);
    ck_assert_ptr_eq(column->orig_name, NULL);
    ck_assert_ptr_eq(column->charset, NULL);
    sql_result_column_free(column);
} END_TEST;

START_TEST(test_sql_result_row_init)
{
    ResultRow *row = sql_result_row_init();
    ck_assert_ptr_eq(row->next, NULL);
    sql_result_row_free(row);
} END_TEST;

START_TEST(test_sql_result_init)
{
    Result *result = sql_result_init();
    ck_assert_ptr_eq(result->columns, NULL);
    ck_assert_ptr_eq(result->rows, NULL);
    ck_assert_int_eq(result->column_count, 0);
    ck_assert_int_eq(result->row_count, 0);
    sql_result_free(result);
} END_TEST;