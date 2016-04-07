#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <assert.h>
#include <squeal.h>
#include <squeal_threads.h>

static void         *start_routine(SquealThread *thread);
static void         squeal_thread_init(ThreadPool *tp, SquealThread thread, int id);
static void         squeal_job_push(ThreadJobQueue *queue, ThreadJob *job);
static int          squeal_job_queue_init(ThreadPool *tp);
static void         squeal_sem_init(SquealSem *sem, unsigned int val);
static ThreadJob   *squeal_job_shift(ThreadJobQueue *queue);
static void         squeal_sem_reset(SquealSem *sem);
static void         squeal_sem_post(SquealSem *sem);
static void         squeal_sem_post_all(SquealSem *sem);
static void         squeal_job_queue_clear(ThreadJobQueue *queue);
static void         squeal_job_queue_destroy(ThreadJobQueue *queue);
static void         squeal_sem_wait(SquealSem *sem);

ThreadPool *squeal_tp_init(unsigned int total_threads)
{
    ThreadPool *tp;
    int i;

    tp = (ThreadPool *) malloc(sizeof(ThreadPool) + sizeof(SquealThread) * total_threads);

    /* can't allocate */
    if (tp == NULL) {
        fprintf(stderr, "Unable to allocate thread pool");
        return NULL;
    }

    tp->paused = 0;
    tp->keep_threads_alive = 1;
    tp->total_threads = total_threads;

    if (squeal_job_queue_init(tp) == -1) {
        free(tp);
        return NULL;
    }

    pthread_mutex_init(&tp->thread_count_lock, NULL);
    pthread_mutex_init(&tp->threads_paused_lock, NULL);
    pthread_cond_init(&tp->threads_idle, NULL);
    pthread_cond_init(&tp->threads_paused_cond, NULL);

    for (i = 0; i < total_threads; i++) {
        squeal_thread_init(tp, tp->threads[i], i);
    }

    /* wait for threads to initialize */
    while (tp->total_threads_alive != total_threads) {}
    return tp;
}

void squeal_tp_destroy(ThreadPool *tp)
{
    double timeout = 1.0;
    double tpassed = 0.0;
    time_t start;
    time_t end;

    if (tp == NULL) {
        return;
    }

    tp->keep_threads_alive = 0;
    time(&start);

    while (tpassed < timeout && tp->total_threads_alive) {
        squeal_sem_post_all(tp->job_queue->has_jobs);
        time(&end);
        tpassed = difftime(end, start);
    }

    while (tp->total_threads_alive) {
        squeal_sem_post_all(tp->job_queue->has_jobs);
        sleep(1);
    }

    squeal_job_queue_destroy(tp->job_queue);

    pthread_mutex_destroy(&tp->thread_count_lock);
    pthread_mutex_destroy(&tp->threads_paused_lock);
    pthread_cond_destroy(&tp->threads_idle);
    pthread_cond_destroy(&tp->threads_paused_cond);

    free(tp->job_queue);
    free(tp);
}

static void squeal_job_queue_destroy(ThreadJobQueue *queue)
{
    squeal_job_queue_clear(queue);
    free(queue->has_jobs);
}

static void squeal_thread_init(ThreadPool *tp, SquealThread thread, int id)
{
    thread.pool = tp;
    thread.id = id;

    pthread_create(&thread.thread, NULL, (void *) start_routine, &thread);
    pthread_detach(thread.thread);
}

void *start_routine(SquealThread *thread)
{
    ThreadPool *tp = thread->pool;

    pthread_mutex_lock(&tp->thread_count_lock);
    tp->total_threads_alive++;
    pthread_mutex_unlock(&tp->thread_count_lock);

    while (tp->keep_threads_alive) {
        squeal_sem_wait(tp->job_queue->has_jobs);

        if (tp->paused) {
            pthread_mutex_lock(&tp->threads_paused_lock);

            while (tp->paused) {
                pthread_cond_wait(&tp->threads_paused_cond, &tp->threads_paused_lock);
            }

            pthread_mutex_unlock(&tp->threads_paused_lock);
        }

        if (tp->keep_threads_alive) {
            pthread_mutex_lock(&tp->thread_count_lock);
            tp->total_threads_working++;
            pthread_mutex_unlock(&tp->thread_count_lock);

            ThreadJob *job;
            pthread_mutex_lock(&tp->job_queue->mutex);
            job = squeal_job_shift(tp->job_queue);
            pthread_mutex_unlock(&tp->job_queue->mutex);

            if (job) {
                job->job_function(job->arg);
                free(job);
            }

            pthread_mutex_lock(&tp->thread_count_lock);
            tp->total_threads_working--;

            if (!tp->total_threads_working) {
                pthread_cond_signal(&tp->threads_idle);
            }

            pthread_mutex_unlock(&tp->thread_count_lock);
        }
    }

    pthread_mutex_lock(&tp->thread_count_lock);
    tp->total_threads_alive--;
    pthread_mutex_unlock(&tp->thread_count_lock);

    return NULL;
}

int squeal_tp_add_work(ThreadPool *tp, void (*job_function)(void *), void *arg)
{
    ThreadJob *job;
    job = (ThreadJob *) malloc(sizeof(ThreadJob));

    if (job == NULL) {
        fprintf(stderr, "Unable to allocate new ThreadJob");
        return -1;
    }

    job->job_function = job_function;
    job->arg = arg;

    pthread_mutex_lock(&tp->job_queue->mutex);
    squeal_job_push(tp->job_queue, job);
    pthread_mutex_unlock(&tp->job_queue->mutex);

    return 0;
}

static void squeal_job_push(ThreadJobQueue *queue, ThreadJob *job)
{
    job->previous = NULL;

    if (queue->len == 0) {
        queue->front = job;
        queue->rear = job;
    } else {
        queue->rear->previous = job;
        queue->rear = job;
    }

    queue->len++;
    squeal_sem_post(queue->has_jobs);
}

static int squeal_job_queue_init(ThreadPool *tp)
{
    tp->job_queue = (ThreadJobQueue *) malloc(sizeof(ThreadJobQueue));

    if (tp->job_queue == NULL) {
        fprintf(stderr, "Unable to allocate job_queue");
        return -1;
    }

    tp->job_queue->len = 0;
    tp->job_queue->front = NULL;
    tp->job_queue->rear = NULL;

    tp->job_queue->has_jobs = (SquealSem *) malloc(sizeof(SquealSem));

    if (tp->job_queue->has_jobs == NULL) {
        fprintf(stderr, "Unable to allocate has_jobs");
        free(tp->job_queue);
        return -1;
    }

    pthread_mutex_init(&tp->job_queue->mutex, NULL);
    squeal_sem_init(tp->job_queue->has_jobs, 0);
    return 0;
}

static void squeal_job_queue_clear(ThreadJobQueue *queue)
{
    while (queue->len) {
        free(squeal_job_shift(queue));
    }

    queue->front = NULL;
    queue->rear = NULL;
    squeal_sem_reset(queue->has_jobs);
    queue->len = 0;
}

static ThreadJob *squeal_job_shift(ThreadJobQueue *queue)
{
    ThreadJob *job;
    job = queue->front;

    if (queue->len == 1) {
        queue->front = NULL;
        queue->rear = NULL;
        queue->len = 0;
    } else if (queue->len > 1) {
        queue->front = job->previous;
        queue->len--;
        squeal_sem_post(queue->has_jobs);
    }

    return job;
}

void squeal_tp_wait(ThreadPool *tp)
{
    pthread_mutex_lock(&tp->thread_count_lock);

    while (tp->job_queue->len || tp->total_threads_working) {
        pthread_cond_wait(&tp->threads_idle, &tp->thread_count_lock);
    }

    pthread_mutex_unlock(&tp->thread_count_lock);
}

void squeal_tp_pause(ThreadPool *tp)
{
    pthread_mutex_lock(&tp->threads_paused_lock);
    tp->paused = 1;
    pthread_mutex_unlock(&tp->threads_paused_lock);
}

void squeal_tp_resume(ThreadPool *tp)
{
    pthread_mutex_lock(&tp->threads_paused_lock);
    tp->paused = 0;
    pthread_cond_broadcast(&tp->threads_paused_cond);
    pthread_mutex_unlock(&tp->threads_paused_lock);
}

static void squeal_sem_init(SquealSem *sem, unsigned int val)
{
    assert(val < 2);

    pthread_mutex_init(&sem->mutex, NULL);
    pthread_cond_init(&sem->condition, NULL);
    sem->val = val;
}

static void squeal_sem_reset(SquealSem *sem)
{
    squeal_sem_init(sem, 0);
}

static void squeal_sem_post(SquealSem *sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->val = 1;
    pthread_cond_signal(&sem->condition);
    pthread_mutex_unlock(&sem->mutex);
}

static void squeal_sem_post_all(SquealSem *sem)
{
    pthread_mutex_lock(&sem->mutex);
    sem->val = 1;
    pthread_cond_broadcast(&sem->condition);
    pthread_mutex_unlock(&sem->mutex);
}

static void squeal_sem_wait(SquealSem *sem)
{
    pthread_mutex_lock(&sem->mutex);

    while (sem->val != 1) {
        pthread_cond_wait(&sem->condition, &sem->mutex);
    }

    sem->val = 0;
    pthread_mutex_unlock(&sem->mutex);
}
