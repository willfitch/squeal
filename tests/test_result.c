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