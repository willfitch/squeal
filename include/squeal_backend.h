#ifndef HAVE_SQUEAL_BACKEND_H
#define HAVE_SQUEAL_BACKEND_H

#include <stdlib.h>
#include <pthread.h>
#include <squeal_types.h>


typedef Result *(SquealQueryHandler)(SqlStatement *statement);



struct _squeal_backend {
    SquealString *internal_name;
    SquealString *external_name;

    SquealQueryHandler *query_handler;
};

void squeal_register_backend(Backend *backend);

#endif