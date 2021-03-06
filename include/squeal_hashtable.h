#ifndef HAVE_SQUEAL_HASHTABLE_H
#define HAVE_SQUEAL_HASHTABLE_H

#include <squeal.h>

#define DEFAULT_HASH_SIZE 32

#define HASHTABLE_TYPE_SVAL (1 << 1)
#define HASHTABLE_TYPE_PTR (2 << 1)

#define SQUEAL_HASHTABLE_ITERATE(ht) \
    do { \
        squeal_ht_record *record; \
        uint32_t total_records = (ht)->ma.mask + 1; \
        (ht)->pos = 0; \
        for (; (ht)->pos < total_records; (ht)->pos++) { \
            record = (ht)->rec[(ht)->pos];

#define SQUEAL_HASHTABLE_ITERATE_SVAL(ht, k, e) \
    SQUEAL_HASHTABLE_ITERATE(ht) \
    if (!(ht)->rec[ht->pos]->is_used) {\
        continue; \
    } \
    if (record->type != HASHTABLE_TYPE_SVAL) { \
        continue; \
    } \
    (e) = record->v.sval; \
    (k) = record->key;

#define SQUEAL_HASHTABLE_ITERATE_PTR(ht, k, e) \
    SQUEAL_HASHTABLE_ITERATE(ht) \
    if (!(ht)->rec[(ht)->pos]->is_used) {\
        continue; \
    } \
    if (record->type != HASHTABLE_TYPE_PTR) { \
        continue; \
    } \
    (e) = record->v.ptr; \
    (k) = record->key;

#define SQUEAL_HASH_ITERATE_END() \
        } \
    } while (0);

typedef struct _squeal_hashtable_record squeal_ht_record;

struct _squeal_hashtable_record {
    uint32_t        id;
    uint32_t        hash;
    SquealString   *key;
    uint32_t        type;
    int             is_used;

    struct {
        sval        *sval;
        void        *ptr; /* used for random pointers */
    } v;
};

/*
 * The idea behind this hashtable loosely follows the paradigm used
 * by Python. The idea is the size of the hashtable is always
 * the power of 2.  Initially, the size is 32 (slots). When the
 * used slots (ma.used) hit >= 67%, the hashtable is reallocated
 * to the next power of 2. The calculation for determining the
 * percentage used is ma.filled / (ma.mask + 1).
 *
 * The ma.mask keeps track of the number of total slots. However,
 * it is always one less than the actual number of slots available. This
 * allows bit masking the slot number based on a record's hash
 * (record_hash & mask). The actual value of total slots is (mask + 1)
 *
 * The number of used slots (actually occupied) is kept up with
 * ma.used. The ma.filled records the total number of slots that are
 * currently occupied, as well as previously occupied slots. When a
 * record is considered "previously occupied", it is not initially
 * freed. This prevents the unnecessary
 */
struct _squeal_hashtable {
    uint32_t            pos;
    long                seed;

    struct {
        uint32_t        used;
        uint32_t        filled;
        uint32_t        mask;
    } ma;

    squeal_ht_record    *rec[1];
};

HashTable *squeal_ht_init();
uint32_t squeal_ht_count_elements(HashTable *ht);
uint32_t squeal_ht_count_ptr_elements(HashTable *ht);
uint32_t squeal_ht_count_sval_elements(HashTable *ht);
int squeal_ht_add_sval(HashTable **ht, SquealString *key, sval *val);
int squeal_ht_add_ptr(HashTable **ht, SquealString *key, void *ptr);
void squeal_ht_remove_key(HashTable **ht, SquealString *key);
void squeal_ht_free(HashTable *ht);
sval *squeal_ht_find_sval(HashTable *ht, SquealString *str);
void *squeal_ht_find_ptr(HashTable *ht, SquealString *str);
squeal_ht_record *squeal_ht_find(HashTable *ht, SquealString *key);

#endif