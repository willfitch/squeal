#ifndef HAVE_SQUEAL_TYPES_H
#define HAVE_SQUEAL_TYPES_H

#define SQUEAL_IPV4 (1 << 1)
#define SQUEAL_IPV6 (1 << 2)

#define SVAL_TYPE_STR (1 << 1)
#define SVAL_TYPE_DOUBLE (1 << 2)
#define SVAL_TYPE_INT (1 << 3)

typedef struct _squeal_string squeal_string;
typedef struct _squeal_server_info squeal_server_info;
typedef struct _squeal_val squeal_val;
typedef struct _squeal_hashtable hashtable;

typedef unsigned long ulong;

struct _squeal_server_info {
    uint16_t port;
    squeal_string *address;
};

struct _squeal_val {
    union {
        struct {
            squeal_string *strval;
            double dval;
            int ival;
        } v;

        uint32_t type;
    } val;
};

#endif
