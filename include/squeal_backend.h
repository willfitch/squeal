#ifndef HAVE_SQUEAL_BACKEND_H
#define HAVE_SQUEAL_BACKEND_H

#include <stdlib.h>
#include <pthread.h>
#include <squeal_types.h>

#define SQUEAL_NAME(name, pfix) squeal_backend_##name_##pfix
#define SQUEAL_BE_INIT(name) Backend *name()

#define SQUEAL_BACKEND_INIT(name) SQUEAL_BE_INIT(SQUEAL_NAME(name, init))
#define SQUEAL_BACKEND_DEINIT(name) SQUEAL_BE_INIT(SQUEAL_NAME(name, deinit))

typedef Result *(SquealQueryHandler)(SqlStatement *statement);


struct _squeal_backend {
    SquealString *internal_name;
    SquealString *external_name;

    SquealQueryHandler *query_handler;
};

void squeal_register_backend(Backend *backend);
Backend *squeal_backend_create(const char *name);

#endif