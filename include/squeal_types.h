#ifndef HAVE_SQUEAL_TYPES_H
#define HAVE_SQUEAL_TYPES_H

typedef struct _squeal_string squeal_string;
typedef struct _squeal_server_info squeal_server_info_t;

struct _squeal_string {
    size_t len;
    char val[1];
};

struct _squeal_server_info {
    uint16_t port;
    squeal_string *address;
};

#endif
