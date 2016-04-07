#ifndef HAVE_SQUEAL_STATS_H
#define HAVE_SQUEAL_STATS_H

#include <stdlib.h>
#include <time.h>
#include <squeal_types.h>
#include <pthread.h>

#define SQUEAL_BACKEND_STAT_INC(be, stat, total) \
    pthread_mutex_lock(&be->stats.lock); \
    be->stats.stat += total; \
    pthread_mutex_unlock(&be->stats.lock);

#define SQUEAL_STAT_INC(ss, type, stat, total) \
    pthread_mutex_lock(&ss.type.lock); \
    ss.type.stat += total; \
    pthread_mutex_unlock(&ss.type.lock);

#define SQUEAL_STAT_DEC(ss, type, stat, total) \
    pthread_mutex_lock(&ss.type.lock); \
    ss.type.stat -= total; \
    pthread_mutex_unlock(&ss.type.lock);

struct _squeal_stats {

    struct {
        pthread_mutex_t lock;
        volatile uint64_t total;
        volatile uint64_t sql_errors;
        volatile uint64_t total_slow;
    } query;

    struct {
        pthread_mutex_t lock;
        volatile uint64_t current;
        volatile uint64_t total;
        volatile uint64_t total_served;
        volatile uint64_t auth_failed;
    } connection;

    volatile int *threads_working; /* reference to thread pool */
    volatile int *threads_alive; /* reference to thread pool */
    volatile int *job_queue_size; /* reference to job queue pool */

    unsigned int start_time;
};

#endif