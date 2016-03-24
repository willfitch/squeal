#ifndef HAVE_SQUEAL_TYPES_H
#define HAVE_SQUEAL_TYPES_H

#define SQUEAL_IPV4 (1 << 1)
#define SQUEAL_IPV6 (1 << 2)

typedef struct _squeal_string squeal_string;
typedef struct _squeal_server_info squeal_server_info;

struct _squeal_string {
    size_t len;

    /* must be the last element! */
    char val[1];
};

struct _squeal_server_info {
    uint16_t port;
    squeal_string *address;
};

#endif
