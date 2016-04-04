#include <squeal.h>
#include <check.h>
#include <unistd.h>

START_TEST(test_squeal_config_init)
{
    SquealConfig *config = squeal_config_init("test.ini");

    ck_assert_str_eq(config->root_section->name->val, "squeald");
    ck_assert_str_eq(config->root_section->root->key->val, "port");
    ck_assert_str_eq(config->root_section->root->v.strval->val, "3306");
    ck_assert_str_eq(config->root_section->next->name->val, "backend");
    ck_assert_str_eq(config->root_section->next->root->key->val, "extension");
    ck_assert_str_eq(config->root_section->next->root->v.strval->val, "elasticsearch.so");

    ck_assert_ptr_eq(config->root_section, config->root_section->next->prev);

    squeal_config_free(config);
} END_TEST;

START_TEST(test_squeal_config_find_entry)
{
    SquealConfig *config = squeal_config_init("test.ini");
    ConfigEntry *entry = squeal_config_find_entry(config, "squeald", "port");

    ck_assert_ptr_ne(entry, NULL);
    ck_assert_str_eq(entry->v.strval->val, "3306");

    squeal_config_free(config);
} END_TEST;

START_TEST(test_S_CONFIG_STR)
{
    SquealConfig *config = squeal_config_init("test.ini");
    SquealString *str = NULL;
    S_CONFIG_STR(config, "squeald", "port", &str);
    ck_assert_str_eq(str->val, "3306");
    squeal_config_free(config);
} END_TEST;

START_TEST(test_S_CONFIG_INT)
{
    SquealConfig *config = squeal_config_init("test.ini");
    int port = 0;
    S_CONFIG_INT(config, "squeald", "port", &port);
    ck_assert_int_eq(port, 3306);
    squeal_config_free(config);
} END_TEST;

START_TEST(test_S_CONFIG_BOOL)
{
    SquealConfig *config = squeal_config_init("test.ini");
    int t_statment = -1;
    int f_statement = -1;

    S_CONFIG_BOOL(config, "squeald", "true_statement", &t_statment);
    ck_assert_int_eq(t_statment, 1);

    S_CONFIG_BOOL(config, "squeald", "false_statement", &f_statement);
    ck_assert_int_eq(f_statement, 0);

    squeal_config_free(config);
} END_TEST;