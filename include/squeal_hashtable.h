#ifndef HAVE_SQUEAL_HASHTABLE_H
#define HAVE_SQUEAL_HASHTABLE_H

#include <squeal.h>

#define DEFAULT_HASH_SIZE 32

#define HASHTABLE_TYPE_SVAL (1 << 1)
#define HASHTABLE_TYPE_PTR (2 << 1)

#define SQUEAL_HASH_ITERATE(ht) \
    squeal_ht_record *record; \
    uint32_t total_records = ht->ma.mask + 1; \
    ht->pos = 0; \
    for (; ht->pos < total_records; ht->pos++) { \
        if (!ht->rec[ht->pos]->is_used) {\
            continue; \
        } \
        record = ht->rec[ht->pos];

#define SQUEAL_HASH_ITERATE_SVAL(ht, entry) \
    SQUEAL_HASH_ITERATE(ht) \
    if (record->type != HASHTABLE_TYPE_SVAL) { \
        continue; \
    } \
    entry = record->v.sval;

#define SQUEAL_HASH_ITERATE_PTR(ht, entry) \
    SQUEAL_HASH_ITERATE(ht) \
    if (record->type != HASHTABLE_TYPE_PTR) { \
        continue; \
    } \
    entry = *record->v.ptr;

#define SQUEAL_HASH_ITERATE_END() \
    } \
    ht->pos = 0;

typedef struct _squeal_hashtable_record squeal_ht_record;

struct _squeal_hashtable_record {
    uint32_t        hash;
    squeal_string   *key;
    uint32_t        type;
    int             is_used;

    struct {
        squeal_val  *sval;
        void        *ptr; /* used for random pointers */
    } v;
};

/*
 * The idea behind this hashtable loosely follows the paradigm used
 * by Python. The idea is the size of the hashtable is always
 * the power of 2.  Initially, the size is 32 (slots). When the
 * used slots (ma.used) hit ~67%, the hashtable is reallocated
 * to the next power of 2. The calculation for determining the
 * percentage used is (ma.filled / ((ma.mask + 1) * .67).
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

hashtable *squeal_ht_init();
int squeal_ht_add_sval(hashtable *ht, squeal_string *key, squeal_val *val);
int squeal_ht_add_ptr(hashtable *ht, squeal_string *key, void *ptr);
void squeal_ht_free(hashtable *ht);
squeal_val *squeal_ht_find_sval(hashtable *ht, squeal_string *str);
void *squeal_ht_find_ptr(hashtable *ht, squeal_string *str);
squeal_ht_record *squeal_ht_find(hashtable *ht, squeal_string *key);

#endif