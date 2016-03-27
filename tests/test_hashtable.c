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
    squeal_val *val;
    SVAL_INIT(val);
    squeal_val *val2;
    val->val.v.ival = 234;

    squeal_string *key = squeal_string_init("testkey", sizeof("testkey"));
    squeal_ht_add_sval(&ht, key, val);

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
    squeal_string *key = squeal_string_init("testkey", sizeof("testkey"));
    squeal_ht_add_ptr(&ht, key, blah);

    another = squeal_ht_find_ptr(ht, key);

    ck_assert_ptr_eq(blah, another);
    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_realloc)
{
    hashtable *ht = squeal_ht_init();

    ck_assert_int_eq(ht->ma.mask + 1, 32);

    /* Add 21 elements */
    int i = 0;

    for (; i < 23; i++) {
        squeal_string *key = squeal_string_alloc(sizeof("key-") + sizeof(int));
        sprintf(key->val, "key-%d", i);
        squeal_val *val;
        SVAL_INIT(val);
        val->val.v.dval = 55.5;
        squeal_ht_add_sval(&ht, key, val);
    }

    ck_assert_int_eq(ht->ma.mask + 1, 32);

    /*Add 1 more element */
    squeal_string *next = squeal_string_init("work", sizeof("work"));
    squeal_val *nextval;
    SVAL_INIT(nextval);

    squeal_ht_add_sval(&ht, next, nextval);

    ck_assert_int_eq(ht->ma.mask + 1, 64);
    squeal_ht_free(ht);
} END_TEST