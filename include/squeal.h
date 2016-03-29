#ifndef HAVE_SQUEAL_GLOBAL_H
#define HAVE_SQUEAL_GLOBAL_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <squeal_types.h>
#include <squeal_string.h>
#include <squeal_config.h>
#include <squeal_hashtable.h>

#define S_STR(sval) sval->val.v->strval->val
#define S_INT(sval) sval->val.v.ival
#define S_DOUBLE(sval) sval->val.v.dval

#define SVAL_INIT(s) do {\
    (s) = (squeal_val *) malloc(sizeof(squeal_val)); \
    (s)->val.v.strval = NULL; \
    } while (0);

#define SVAL_STR(s, x) \
    SVAL_INIT((s)); do {\
    (s)->val.v->strval = squeal_string_init((x), sizeof((x))); \
    } while (0);

#define SVAL_PTR(s, x) \
    SVAL_INIT((s)); do {\
    (s)->val.v->ptr = (x) \
    } while (0);

/* server info functionality */
squeal_server_info *squeal_server_info_init(char *addr, uint16_t port);
void squeal_server_info_free(squeal_server_info *server_info);

static inline void squeal_sval_free(squeal_val *val)
{
    if (val) {
        if (val->val.v.strval) {
            squeal_string_free(val->val.v.strval);
        }

        free(val);
    }
}

#endif
