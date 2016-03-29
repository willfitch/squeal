#ifndef HAVE_SQUEAL_SVAL_H
#define HAVE_SQUEAL_SVAL_H

#include <stdlib.h>
#include <squeal_types.h>
#include <squeal_string.h>

#define S_STR(sval) sval->val.v->strval->val
#define S_INT(sval) sval->val.v.ival
#define S_DOUBLE(sval) sval->val.v.dval

#define SVAL_INIT(s) do {\
    (s) = (sval *) malloc(sizeof(sval)); \
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
ServerInfo *squeal_server_info_init(char *addr, uint16_t port);
void squeal_server_info_free(ServerInfo *server_info);

static inline void squeal_sval_free(sval *val)
{
    if (val) {
        if (val->val.v.strval) {
            squeal_string_free(val->val.v.strval);
        }

        free(val);
    }
}

#endif