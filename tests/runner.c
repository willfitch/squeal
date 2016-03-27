#include <check.h>
#include "test_hashtable.c"

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
    suite_add_tcase(suite, ht_case);

    /* test runner */
    SRunner *runner = srunner_create(suite);
    srunner_run_all(runner, CK_NORMAL);
    number_failed = srunner_ntests_failed(runner);
    srunner_free(runner);
    return number_failed;
}