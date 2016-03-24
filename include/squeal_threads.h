#ifndef HAVE_SQUEAL_THREADS_H
#define HAVE_SQUEAL_THREADS_H

#include <pthread.h>

typedef struct _squeal_thread squeal_thread;
typedef struct _squeal_job squeal_job;
typedef struct _squeal_job_queue squeal_job_queue;
typedef struct _squeal_thread_pool squeal_thread_pool;

struct _squeal_thread {
    int id;
    pthread_t thread;
    squeal_thread_pool *pool;
};

struct _squeal_job {
};

struct _squeal_job_queue {
};

struct _squeal_thread_pool{
    int total_threads;

    volatile int total_alive;
    volatile int total_working;
    pthread_mutex_t thread_count_lock;
    pthread_cond_t threads_idle;

    /* must be the last element! */
    squeal_thread threads[1];
};



#endif
