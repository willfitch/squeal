#ifndef HAVE_SQUEAL_CONFIG_H
#define HAVE_SQUEAL_CONFIG_H

#include <stdlib.h>
#include <squeal_types.h>

#define S_CONFIG_STR(c, s, k, p) do {\
    ConfigEntry *__sec = squeal_config_find_entry(c, s, k); \
    if (__sec) {\
        *p = __sec->v.strval; \
    } \
    } while (0);

#define S_CONFIG_INT(c, s, k, i) do {\
    ConfigEntry *__sec = squeal_config_find_entry(c, s, k); \
    if (__sec) {\
        *i = atoi(__sec->v.strval->val); \
    } \
    } while (0);

#define S_CONFIG_BOOL(c, s, k, b) do {\
    ConfigEntry *__sec = squeal_config_find_entry(c, s, k); \
    if (__sec) {\
        if (strcmp(__sec->v.strval->val, "true") == 0) {\
            *b = 1; \
        } else if (strcmp(__sec->v.strval->val, "false") == 0) { \
            *b = 0; \
        } \
    } \
    } while (0);


struct _squeal_ini_entry
{
    SquealString *key; /* the key name */

    union {
        SquealString *strval;
        int ival;
        double dval;
    } v; /* entry values */

    ConfigEntry *next; /* pointer to the next entry */
    ConfigEntry *prev; /* pointer to the previous entry */
};

struct _squeal_ini_section
{
    SquealString *name; /* name of the section */
    ConfigEntry *root; /* root ConfigEntry */

    ConfigSection *next; /* pointer to the next section */
    ConfigSection *prev; /* pointer to the previous section */
};

struct _squeal_ini
{
    SquealString *file; /* path/name of the ini file */
    ConfigSection *root_section; /* base ini section */
};

SquealConfig *squeal_config_init(const char *file);
ConfigEntry *squeal_config_find_entry(SquealConfig *config, const char *section, const char *key);
void squeal_config_free(SquealConfig *config);

#endif