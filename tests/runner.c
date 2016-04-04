#include <check.h>
#include "test_hashtable.c"
#include "test_function.c"
#include "test_string.c"
#include "test_squeal.c"
#include "test_result.c"
#include "test_config.c"

int main(int argc, char *argv[])
{
    int number_failed;

    Suite *suite = suite_create("squeal");

    /* hashtable tests */
    TCase *ht_case = tcase_create("hashtable");
    tcase_add_test(ht_case, test_hashtable_init);
    tcase_add_test(ht_case, test_hashtable_add_sval);
    tcase_add_test(ht_case, test_hashtable_add_ptr);
    tcase_add_test(ht_case, test_hashtable_realloc);
    tcase_add_test(ht_case, test_hashtable_remove_key);
    tcase_add_test(ht_case, test_SQUEAL_HASH_ITERATE_SVAL);
    tcase_add_test(ht_case, test_SQUEAL_HASH_ITERATE_PTR);
    suite_add_tcase(suite, ht_case);

    TCase *func_case = tcase_create("function");
    tcase_add_test(func_case, test_function_init);
    tcase_add_test(func_case, test_function_param_init);
    tcase_add_test(func_case, test_sql_function_param_init);
    tcase_add_test(func_case, test_sql_function_add_param);
    suite_add_tcase(suite, func_case);

    TCase *str_case = tcase_create("string");
    tcase_add_test(str_case, test_squeal_string_init);
    tcase_add_test(str_case, test_squeal_string_alloc);
    tcase_add_test(str_case, test_squeal_string_realloc);
    tcase_add_test(str_case, test_squeal_string_empty);
    tcase_add_test(str_case, test_squeal_string_equals);
    suite_add_tcase(suite, str_case);

    TCase *squeal_case = tcase_create("squeal");
    tcase_add_test(squeal_case, test_sql_select_init);
    tcase_add_test(squeal_case, test_sql_table_init);
    tcase_add_test(squeal_case, test_sql_order_by_init);
    tcase_add_test(squeal_case, test_sql_group_by_init);
    tcase_add_test(squeal_case, test_sql_column_init);
    tcase_add_test(squeal_case, test_sql_where_init);
    tcase_add_test(squeal_case, test_sql_statement_init);
    suite_add_tcase(suite, squeal_case);

    TCase *result_case = tcase_create("result");
    tcase_add_test(result_case, test_sql_result_column_init);
    tcase_add_test(result_case, test_sql_result_row_init);
    tcase_add_test(result_case, test_sql_result_init);
    suite_add_tcase(suite, result_case);

    TCase *config_case = tcase_create("config");
    tcase_add_test(config_case, test_squeal_config_init);
    tcase_add_test(config_case, test_squeal_config_find_entry);
    tcase_add_test(config_case, test_S_CONFIG_STR);
    tcase_add_test(config_case, test_S_CONFIG_INT);
    tcase_add_test(config_case, test_S_CONFIG_BOOL);
    suite_add_tcase(suite, config_case);

    /* test runner */
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed;
}