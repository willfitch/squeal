#include <squeal_backend.h>
#include <squeal_hashtable.h>
#include <pthread.h>
#include <dlfcn.h>

typedef struct {
    HashTable *backends;
    pthread_mutex_t wlock;
} BackendManagement;

BackendManagement bm;

void squeal_register_backend(Backend *backend)
{
    if (backend == NULL) {
        fprintf(stderr, "squeal_register_backend: Backend is null");
        exit(EXIT_FAILURE);
    }

    if (backend->internal_name == NULL) {
        fprintf(stderr, "squeal_register_backend: Backend internal name is null");
        exit(EXIT_FAILURE);
    }

    if (backend->external_name == NULL) {
        backend->external_name = backend->internal_name;
    }

    pthread_mutex_lock(&bm.wlock);
    squeal_ht_add_ptr(&bm.backends, backend->internal_name, (void *) backend);
    pthread_mutex_unlock(&bm.wlock);
}

Backend *squeal_backend_create(const char *name)
{
    Backend *be = (Backend *) malloc(sizeof(Backend));

    if (be == NULL) {
        fprintf(stderr, "squeal_backend_create: unable to allocate backdnd");
        exit(EXIT_FAILURE);
    }

    be->query_handler = NULL;
    be->internal_name = squeal_string_init(name, sizeof(name));
    be->external_name = be->internal_name;

    return be;
}

void squeal_backend_init()
{
    bm.backends = squeal_ht_init();
    pthread_mutex_init(&bm.wlock, NULL);
}

void squeal_backend_deinit()
{
    /* give backends a chance to de-initialize */
    if (bm.backends != NULL) {
        pthread_mutex_destroy(&bm.wlock);
    }

    squeal_ht_free(bm.backends);
}