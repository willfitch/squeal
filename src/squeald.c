#include <squeal.h>


void task1() {
    printf("Task1 executed\n");
}


void task2() {
    printf("Task2 executed\n");
}

int main()
{
    printf("Good to go\n");/*
    squeal_thread_pool *pool = squeal_tp_init(6);

    printf("Adding 40 tasks\n");

    int i;

    for (i = 0; i < 40; i++) {
        squeal_tp_add_work(pool, (void *) task1, NULL);
        squeal_tp_add_work(pool, (void *) task2, NULL);
    }

    squeal_tp_destroy(pool);*/

    hashtable *ht = squeal_ht_init();
    squeal_val *blah = SVAL_INIT();
    blah->val.v.dval = 55.5;

    squeal_val *val;
    squeal_string *key = squeal_string_init("test", strlen("test"));
    squeal_ht_add_sval(ht, key, blah);

    SQUEAL_HASH_ITERATE_SVAL(ht, val) {
        printf("d val is %f\n", val->val.v.dval);
    } SQUEAL_HASH_ITERATE_END();

    printf("doing a search\n");


    val = squeal_ht_find_sval(ht, key);

    if (!val) {
        printf("could not find it\n");
    } else {
        printf("found: %f\n", val->val.v.dval);
    }

    squeal_string_free(key);

    printf("it is %lu\n", ht->seed);
}