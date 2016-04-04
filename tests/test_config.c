#include <squeal.h>
#include <check.h>

START_TEST(test_squeal_config_init)
{
    SquealConfig *config = squeal_config_init("/Users/wfitch/projects/csqueal/tests/test.ini");

    ck_assert_str_eq(config->root_section->name->val, "squeald");
    ck_assert_str_eq(config->root_section->root->key->val, "port");
    ck_assert_str_eq(config->root_section->root->v.strval->val, "3306");
    ck_assert_str_eq(config->root_section->next->name->val, "backend");
    ck_assert_str_eq(config->root_section->next->root->key->val, "extension");
    ck_assert_str_eq(config->root_section->next->root->v.strval->val, "elasticsearch.so");

    ck_assert_ptr_eq(config->root_section, config->root_section->next->prev);

    printf("here: %s and %lu\n", config->root_section->root->key->val, config->root_section->root->key->len);

    squeal_config_free(config);
} END_TEST;