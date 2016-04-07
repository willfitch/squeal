#ifndef HAVE_SQUEAL_THREADS_H
#define HAVE_SQUEAL_THREADS_H

#include <pthread.h>

typedef struct _squeal_job_queue_status SquealSem;
typedef struct _squeal_thread SquealThread;
typedef struct _squeal_job ThreadJob;
typedef struct _squeal_job_queue ThreadJobQueue;
typedef struct _squeal_thread_pool ThreadPool;

struct _squeal_thread {
    int id;
    pthread_t thread;
    ThreadPool *pool;
};

struct _squeal_job_queue_status {
    pthread_mutex_t mutex;
    pthread_cond_t condition;
    int val;
};

struct _squeal_job {
    ThreadJob *previous;
    void (*job_function)(void *arg);
    void *arg;
};

struct _squeal_job_queue {
    unsigned int len;
    pthread_mutex_t mutex;
    ThreadJob *front;
    ThreadJob *rear;
    SquealSem *has_jobs;
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

    ThreadJobQueue *job_queue;

    /* must be the last element! */
    SquealThread threads[1];
};

ThreadPool *squeal_tp_init(unsigned int total_threads);
int squeal_tp_add_work(ThreadPool *tp, void (*job_function)(void *), void *arg);
void squeal_tp_pause(ThreadPool *tp);
void squeal_tp_resume(ThreadPool *tp);
void squeal_tp_destroy(ThreadPool *tp);
void squeal_tp_wait(ThreadPool *tp);

#endif
