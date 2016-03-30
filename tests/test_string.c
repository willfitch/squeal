#include <check.h>
#include <squeal.h>

START_TEST(test_squeal_string_init)
{
    SquealString *str = squeal_string_init("test", sizeof("test"));
    ck_assert_str_eq(str->val, "test");
    squeal_string_free(str);
} END_TEST

START_TEST(test_squeal_string_alloc)
{
    SquealString *str = squeal_string_alloc(sizeof("test"));
    ck_assert_int_eq(sizeof("test"), str->len);
    strcpy(str->val, "test");
    ck_assert_str_eq("test", str->val);
    squeal_string_free(str);
} END_TEST

START_TEST(test_squeal_string_realloc)
{
    SquealString *str = squeal_string_init("test", sizeof("test"));
    ck_assert_str_eq("test", str->val);
    squeal_string_realloc(&str, sizeof("test") + sizeof(char));
    memcpy(str->val, "test1", sizeof("test1"));
    ck_assert_str_eq("test1", str->val);
    squeal_string_free(str);
} END_TEST

START_TEST(test_squeal_string_empty)
{
    SquealString *str = squeal_string_init("test", sizeof("test"));
    squeal_string_empty(str);
    ck_assert_str_eq("\0\0\0\0\0", str->val);
    squeal_string_free(str);
} END_TEST

START_TEST(test_squeal_string_equals)
{
    SquealString *str1 = squeal_string_init("test", sizeof("test"));
    SquealString *str2 = squeal_string_init("test2", sizeof("test2"));
    SquealString *str3 = squeal_string_init("test", sizeof("test"));
    ck_assert_int_eq(0, squeal_string_equals(str1, str2));
    ck_assert_int_eq(1, squeal_string_equals(str1, str3));
    squeal_string_free(str1);
    squeal_string_free(str2);
    squeal_string_free(str3);
} END_TEST
