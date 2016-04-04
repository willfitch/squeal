#ifndef HAVE_SQUEAL_CONFIG_H
#define HAVE_SQUEAL_CONFIG_H

#include <stdlib.h>
#include <squeal_types.h>

#define INI_TYPE_STR (1 << 1)
#define INI_TYPE_INT (1 << 2)
#define INI_TYPE_DOUBLE (1 << 3)

struct _squeal_ini_entry
{
    SquealString *key; /* the key name */
    uint16_t type; /* the type of entry */

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
void squeal_config_free(SquealConfig *config);

#endif