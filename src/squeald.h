#ifndef HAVE_SQUEALD_H
#define HAVE_SQUEALD_H

#include <squeal.h>

typedef struct {
    ServerInfo *server_info; /* Server information */
    Client *client[1]; /* Client connections */
} squeald;

#endif