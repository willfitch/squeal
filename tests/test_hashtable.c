#include <check.h>
#include <squeal.h>

START_TEST(test_hashtable_init)
{
    hashtable *ht = squeal_ht_init();
    ck_assert_msg(ht != NULL, "unable to allocate hashtable");
    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_add_sval)
{
    hashtable *ht = squeal_ht_init();
    squeal_val *val = SVAL_INIT();
    squeal_val *val2;
    val->val.v.ival = 234;

    squeal_string *key = squeal_string_init("testkey", strlen("testkey"));
    squeal_ht_add_sval(ht, key, val);

    val2 = squeal_ht_find_sval(ht, key);
    ck_assert_ptr_eq(val, val2);

    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_add_ptr)
{
    hashtable *ht = squeal_ht_init();
    char *blah = malloc(sizeof("willy"));
    char *another;
    strncpy(blah, "willy", sizeof("willy"));
    squeal_string *key = squeal_string_init("testkey", strlen("testkey"));
    squeal_ht_add_ptr(ht, key, blah);

    another = squeal_ht_find_ptr(ht, key);

    ck_assert_ptr_eq(blah, another);
    free(blah);
    squeal_string_free(key);
    squeal_ht_free(ht);
} END_TEST