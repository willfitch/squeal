#include <check.h>
#include <squeal.h>
#include <squeal_hashtable.h>

START_TEST(test_hashtable_init)
{
    HashTable *ht = squeal_ht_init();
    ck_assert_msg(ht != NULL, "unable to allocate hashtable");
    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_add_sval)
{
    HashTable *ht = squeal_ht_init();
    sval *val;
    SVAL_INIT(val);
    sval *val2;
    val->val.v.ival = 234;

    SquealString *key = squeal_string_init("testkey", sizeof("testkey"));
    squeal_ht_add_sval(&ht, key, val);

    val2 = squeal_ht_find_sval(ht, key);
    ck_assert_ptr_eq(val, val2);

    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_add_ptr)
{
    HashTable *ht = squeal_ht_init();
    char *blah = malloc(sizeof("willy"));
    char *another;
    strncpy(blah, "willy", sizeof("willy"));
    SquealString *key = squeal_string_init("testkey", sizeof("testkey"));
    squeal_ht_add_ptr(&ht, key, blah);

    another = squeal_ht_find_ptr(ht, key);

    ck_assert_ptr_eq(blah, another);
    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_realloc)
{
    HashTable *ht = squeal_ht_init();

    ck_assert_int_eq(ht->ma.mask + 1, 32);

    /* Add 21 elements */
    int i = 0;

    for (; i < 23; i++) {
        SquealString *key = squeal_string_alloc(sizeof("key-") + sizeof(int));
        sprintf(key->val, "key-%d", i);
        sval *val;
        SVAL_INIT(val);
        val->val.v.dval = 55.5;
        squeal_ht_add_sval(&ht, key, val);
    }

    ck_assert_int_eq(ht->ma.mask + 1, 32);

    /*Add 1 more element */
    SquealString *next = squeal_string_init("work", sizeof("work"));
    sval *nextval;
    SVAL_INIT(nextval);

    squeal_ht_add_sval(&ht, next, nextval);

    ck_assert_int_eq(ht->ma.mask + 1, 64);
    squeal_ht_free(ht);
} END_TEST

START_TEST(test_hashtable_remove_key)
{
    HashTable *ht = squeal_ht_init();

    sval *sqval;
    SVAL_INIT(sqval);

    sqval->val.v.dval = 55.5;
    SquealString *str = squeal_string_init("asdf", sizeof("asdf"));

    squeal_ht_add_sval(&ht, str, sqval);

    sval *copy = squeal_ht_find_sval(ht, str);

    ck_assert_ptr_eq(copy, sqval);
    squeal_ht_remove_key(&ht, str);
    copy = squeal_ht_find_sval(ht, str);

    ck_assert_msg(copy == NULL, "");

    squeal_ht_free(ht);

} END_TEST

START_TEST(test_SQUEAL_HASH_ITERATE_SVAL)
{
    HashTable *ht = squeal_ht_init();

    sval *sqval;
    sval *sval2;
    SquealString *keycopy;

    char *blah = malloc(sizeof(char) * 5);
    strncpy(blah, "will", sizeof("will"));
    SVAL_INIT(sqval);

    sqval->val.v.ival = 55;

    SquealString *key = squeal_string_init("key", sizeof("key"));
    SquealString *another = squeal_string_init("key2", sizeof("key2"));

    squeal_ht_add_sval(&ht, key, sqval);
    squeal_ht_add_ptr(&ht, another, blah);

    int found = 0;

    SQUEAL_HASHTABLE_ITERATE_SVAL(ht, keycopy, sval2)
    {
        found++;
    } SQUEAL_HASH_ITERATE_END();

    ck_assert_int_eq(1, found);

    squeal_ht_remove_key(&ht, key);

    found = 0;

    SQUEAL_HASHTABLE_ITERATE_SVAL(ht, keycopy, sval2)
    {
        found++;
    } SQUEAL_HASH_ITERATE_END();

    ck_assert_int_eq(0, found);

    free(blah);
    squeal_ht_free(ht);
} END_TEST

START_TEST(test_SQUEAL_HASH_ITERATE_PTR)
{
    HashTable *ht = squeal_ht_init();

    sval *sqval;
    void *other = NULL;
    SquealString *keycopy;

    char *blah = malloc(sizeof(char) * 5);
    strncpy(blah, "will", sizeof("will"));
    SVAL_INIT(sqval);

    sqval->val.v.ival = 55;

    SquealString *key = squeal_string_init("key", sizeof("key"));
    SquealString *another = squeal_string_init("key2", sizeof("key2"));

    squeal_ht_add_sval(&ht, key, sqval);
    squeal_ht_add_ptr(&ht, another, blah);

    int found = 0;

    SQUEAL_HASHTABLE_ITERATE_PTR(ht, keycopy, other)
    {
        found++;
    } SQUEAL_HASH_ITERATE_END();

    ck_assert_int_eq(1, found);

    squeal_ht_remove_key(&ht, another);

    found = 0;

    SQUEAL_HASHTABLE_ITERATE_PTR(ht, keycopy, other)
    {
        found++;
    } SQUEAL_HASH_ITERATE_END();

    ck_assert_int_eq(0, found);

    free(blah);
    squeal_ht_free(ht);
} END_TEST