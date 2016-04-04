#include <squeal.h>
#include <squeal_config.h>
#include <ctype.h>

static void load_ini(FILE *fp, SquealConfig **config);
static void config_section_init(ConfigSection **s);
static void config_entry_init(ConfigEntry **e);
static void trim(char *s);

SquealConfig *squeal_config_init(const char *file)
{
    SquealConfig *config = (SquealConfig *) malloc(sizeof(SquealConfig));

    if (config == NULL) {
        fprintf(stderr, "squeal_config_init: unable to allocate config");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(file, "r");

    if (fp == NULL) {
        fprintf(stderr, "squeal_config_init: config file %s specified is not readable or does not exist", file);
        exit(EXIT_FAILURE);
    }

    config->file = squeal_string_init(file, sizeof(file));
    load_ini(fp, &config);
    fclose(fp);

    return config;
}

ConfigEntry *squeal_config_find_entry(SquealConfig *config, const char *section, const char *key)
{
    ConfigEntry *entry = NULL;
    ConfigSection *search_section = NULL;

    if (config) {
        search_section = config->root_section;

        while (search_section != NULL) {
            if (strcmp(search_section->name->val, section) == 0) {
                break;
            }

            search_section = search_section->next;
        }
    }

    if (search_section) {
        entry = search_section->root;

        while (entry != NULL) {
            if (strcmp(entry->key->val, key) == 0) {
                break;
            }

            entry = entry->next;
        }
    }

    return entry;
}

static void load_ini(FILE *fp, SquealConfig **config)
{
    SquealConfig *tmp = *config;
    int buff_size = 1024;
    char buffer[buff_size];
    tmp->root_section = (ConfigSection *) malloc(sizeof(ConfigSection));
    config_section_init(&tmp->root_section);

    ConfigSection *curr_section = tmp->root_section;
    ConfigSection *prev_section = NULL;

    ConfigEntry *curr_entry = NULL;
    ConfigEntry *prev_entry = NULL;

    while(fgets(buffer, buff_size, fp) != NULL) {
        trim(buffer);

        if (buffer[0] == '[') { /* section */
            int i = 0, si = 0;
            char section_buff[buff_size];
            memset(section_buff, '\0', buff_size);

            if (curr_section->name != NULL) {
                prev_section = curr_section;
                curr_section->next = (ConfigSection *) malloc(sizeof(ConfigSection));

                if (curr_section->next == NULL) {
                    fprintf(stderr, "load_ini: unable to allocate section");
                    exit(EXIT_FAILURE);
                }

                config_section_init(&curr_section->next);

                curr_section = curr_section->next;
                curr_section->prev = prev_section;
            }

            while (i < buff_size && buffer[i] != '\0'
                    && buffer[i] != '\r' && buffer[i] != '\n') {
                switch (buffer[i]) {
                    case '[':
                    case ']':
                    case ' ':
                        break;
                    default:
                        section_buff[si] = buffer[i];
                        si++;
                        break;
                }

                i++;
            }

            section_buff[si] = '\0';

            curr_section->name = squeal_string_init(section_buff, strlen(section_buff) + 1);
            curr_section->root = (ConfigEntry *) malloc(sizeof(ConfigEntry));

            if (curr_section->root == NULL) {
                fprintf(stderr, "load_ini: unable to allocate section entry root");
                exit(EXIT_FAILURE);
            }

            config_entry_init(&curr_section->root);
            curr_entry = curr_section->root;

        } else if (buffer[0] == ';') { /* comment */
            continue;
        } else if (isalnum(buffer[0])) { /* entry */
            int i = 0, vi = 0, ki = 0;
            short in_key = 1;
            char key_buff[buff_size];
            char val_buff[buff_size];
            memset(key_buff, '\0', buff_size);
            memset(val_buff, '\0', buff_size);

            if (curr_entry->key != NULL) {
                prev_entry = curr_entry;
                curr_entry->next = (ConfigEntry *) malloc(sizeof(ConfigEntry));

                if (curr_entry->next == NULL) {
                    fprintf(stderr, "load_ini: unable to allocate entry");
                    exit(EXIT_FAILURE);
                }

                config_entry_init(&curr_entry->next);

                curr_entry = curr_entry->next;
                curr_entry->prev = prev_entry;
            }

            while (i < buff_size && buffer[i] != '\0'
                   && buffer[i] != '\n' && buffer[i] != '\r') {
                if (!in_key) {
                    val_buff[vi] = buffer[i];
                    vi++;
                } else {
                    if (buffer[i] != ' ' && buffer[i] != '=') {
                        key_buff[ki] = buffer[i];
                    } else if (buffer[i] == '=') {
                        in_key = 0;
                        key_buff[ki] = '\0';
                    }

                    ki++;
                }

                i++;
            }


            val_buff[vi] = '\0';
            trim(val_buff);

            curr_entry->key = squeal_string_init(key_buff, strlen(key_buff) + 1);
            curr_entry->v.strval = squeal_string_init(val_buff, strlen(val_buff) + 1);
        } else if (buffer[0] == '\r' || buffer[0] == '\n' || buffer[0] == '\0') { /* new lines */
            continue;
        } else { /* invalid syntax */
            fprintf(stderr, "Invalid syntax in INI file");
            free(tmp->root_section);
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }
}

static squeal_always_inline void config_section_init(ConfigSection **s)
{
    (*s)->next = NULL;
    (*s)->prev = NULL;
    (*s)->root = NULL;
    (*s)->name = NULL;
}

static squeal_always_inline void config_entry_init(ConfigEntry **e)
{
    (*e)->next = NULL;
    (*e)->key = NULL;
    (*e)->prev = NULL;
    (*e)->v.strval = NULL;
}

static inline void trim(char *s)
{
    char *p = s;
    size_t l = strlen(p);

    while(isspace(p[l - 1])) p[--l] = 0;
    while(*p && isspace(*p)) ++p, --l;

    memmove(s, p, l + 1);
}

void squeal_config_free(SquealConfig *config)
{
    if (config) {
        if (config->root_section) {
            ConfigSection *section = config->root_section;
            ConfigSection *tmp = NULL;

            while (section != NULL) {
                if (section->name) {
                    squeal_string_free(section->name);
                }

                if (section->root) {
                    ConfigEntry *entry = section->root;
                    ConfigEntry *tmpe = NULL;

                    while (entry != NULL) {
                        if (entry->key) {
                            squeal_string_free(entry->key);
                        }

                        if (entry->v.strval) {
                            squeal_string_free(entry->v.strval);
                        }

                        tmpe = entry;
                        entry = entry->next;
                        free(tmpe);
                    }
                }

                tmp = section;
                section = section->next;
                free(tmp);
            }
        }

        free(config);
    }
}