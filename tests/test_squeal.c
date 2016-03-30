#include <check.h>
#include <squeal.h>

START_TEST(test_sql_statement_init)
{
    SqlStatement *statement = sql_statement_init();
    ck_assert_ptr_eq(statement->raw_query, NULL);
    ck_assert_ptr_eq(statement->select_stmt, NULL);
    sql_statement_free(statement);
} END_TEST;

START_TEST(test_sql_select_init)
{
    Select *select = sql_select_init();

    ck_assert_ptr_eq(select->columns, NULL);
    ck_assert_ptr_eq(select->where, NULL);
    ck_assert_ptr_eq(select->group, NULL);
    ck_assert_ptr_eq(select->order, NULL);
    ck_assert_ptr_eq(select->tables, NULL);

    sql_select_free(select);
} END_TEST;

START_TEST(test_sql_table_init)
{
    Table *table = sql_table_init();

    ck_assert_ptr_eq(table->alias, NULL);
    ck_assert_ptr_eq(table->name, NULL);
    ck_assert_ptr_eq(table->next, NULL);
    ck_assert_ptr_eq(table->prev, NULL);

    sql_table_free(table);
} END_TEST;

START_TEST(test_sql_order_by_init)
{
    OrderBy *order = sql_order_by_init();
    ck_assert_ptr_eq(order->columns, NULL);
    sql_order_by_free(order);
} END_TEST;

START_TEST(test_sql_group_by_init)
{
    GroupBy *group = sql_group_by_init();
    ck_assert_ptr_eq(group->columns, NULL);
    sql_group_by_free(group);
} END_TEST;

START_TEST(test_sql_column_init)
{
    Column *column = sql_column_init();
    ck_assert_ptr_eq(column->alias, NULL);
    ck_assert_ptr_eq(column->name, NULL);
    ck_assert_ptr_eq(column->next, NULL);
    ck_assert_ptr_eq(column->prev, NULL);
    ck_assert_ptr_eq(column->v.function, NULL);
    ck_assert_ptr_eq(column->v.sval, NULL);
    sql_column_free(column);
} END_TEST;

START_TEST(test_sql_where_init)
{
    Where *where = sql_where_init();
    ck_assert_ptr_eq(where->child, NULL);
    ck_assert_ptr_eq(where->left, NULL);
    ck_assert_ptr_eq(where->right, NULL);
    ck_assert_ptr_eq(where->next, NULL);
    ck_assert_ptr_eq(where->prev, NULL);
    ck_assert_ptr_eq(where->values, NULL);
    sql_where_free(where);
} END_TEST;