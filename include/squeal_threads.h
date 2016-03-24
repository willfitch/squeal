#ifndef HAVE_SQUEAL_THREADS_H
#define HAVE_SQUEAL_THREADS_H

#include <pthread.h>

typedef struct _squeal_job_queue_status squeal_sem;
typedef struct _squeal_thread squeal_thread;
typedef struct _squeal_job squeal_job;
typedef struct _squeal_job_queue squeal_job_queue;
typedef struct _squeal_thread_pool squeal_thread_pool;

struct _squeal_thread {
    int id;
    pthread_t thread;
    squeal_thread_pool *pool;
};

struct _squeal_job_queue_status {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int val;
};

struct _squeal_job {
    squeal_job *previous;
    void (*job_function)(void *arg);
    void *arg;
};

struct _squeal_job_queue {
    unsigned int len;
    pthread_mutex_t mutex;
    squeal_job *front;
    squeal_job *rear;
    squeal_sem *has_jobs;
};

struct _squeal_thread_pool{
    int total_threads;

    volatile int paused;

    pthread_mutex_t threads_paused_lock;
    pthread_cond_t threads_paused_cond;

    volatile int keep_threads_alive;
    volatile int total_threads_alive;
    volatile int total_threads_working;
    pthread_mutex_t thread_count_lock;
    pthread_cond_t threads_idle;

    squeal_job_queue *job_queue;

    /* must be the last element! */
    squeal_thread threads[1];
};

squeal_thread_pool *squeal_tp_init(unsigned int total_threads);
int squeal_tp_add_work(squeal_thread_pool *tp, void (*job_function)(void *), void *arg);
void squeal_tp_pause(squeal_thread_pool *tp);
void squeal_tp_resume(squeal_thread_pool *tp);
void squeal_tp_destroy(squeal_thread_pool *tp);
void squeal_tp_wait(squeal_thread_pool *tp);

#endif
