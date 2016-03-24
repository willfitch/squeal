#include <squeal_global.h>
#include "squeald.h"
#include <squeal_threads.h>
#include <time.h>
#include <unistd.h>

void task1() {
    fprintf(stdout, "Task1 executed\n");
}


void task2() {
    fprintf(stdout, "Task2 executed\n");
}

int main()
{
    printf("Good to go\n");
    squeal_thread_pool *pool = squeal_tp_init(2);

    printf("Adding 40 tasks\n");

    int i;

    for (i = 0; i < 40; i++) {
        squeal_tp_add_work(pool, (void *) task1, NULL);
        squeal_tp_add_work(pool, (void *) task2, NULL);
    }

    squeal_tp_destroy(pool);
}