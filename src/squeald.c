#include <squeal.h>


void task1() {
    printf("Task1 executed\n");
}


void task2() {
    printf("Task2 executed\n");
}

int main()
{
    printf("Good to go\n");
    hashtable *ht = squeal_ht_init();

    /* Add 21 elements */
    int i = 0;

    for (; i < 32; i++) {
        squeal_string *key = squeal_string_alloc(sizeof("key-") + sizeof(int));
        sprintf(key->val, "key-%d", i);
        squeal_val *val;
        SVAL_INIT(val);
        val->val.v.dval = 55.5;
        squeal_ht_add_sval(&ht, key, val);
    }

    printf("total is %d\n", ht->ma.mask + 1);

    /*Add 1 more element */
    squeal_string *next = squeal_string_init("work", sizeof("work"));
    squeal_val *nextval;
    SVAL_INIT(nextval);

    printf("here we go...\n");
    squeal_ht_add_sval(&ht, next, nextval);

    printf("total is %d\n", ht->ma.mask + 1);

    squeal_ht_free(ht);
}