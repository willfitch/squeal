#include <squeal_hashtable.h>

#define ROT32(x, y) ((x << y) | (x >> (32 - y)))

static uint32_t hash(const char *key, uint32_t len, uint32_t seed);
static long rand_seed(long max);
static uint32_t squeal_ht_find_next_slot(hashtable *ht, uint32_t hash);
static int squeal_check_ht_size(hashtable *ht);
static int squeal_realloc_ht(hashtable *ht);


int squeal_ht_init(hashtable *ht)
{
    int i = 0;
    ht = (hashtable *) malloc(sizeof(hashtable) + sizeof(squeal_ht_record) * DEFAULT_HASH_SIZE);

    if (ht == NULL) {
        fprintf(stderr, "squeal_ht_init: unable to allocate hashtable");
        return -1;
    }

    ht->ma.mask = DEFAULT_HASH_SIZE - 1;
    ht->ma.filled = 0;
    ht->ma.used = 0;
    ht->pos = 0;
    ht->seed = (uint32_t) rand_seed(100);

    /* Allocate the initial slots */
    for (; i < DEFAULT_HASH_SIZE; i++) {
        ht->rec[i] = (squeal_ht_record *) malloc(sizeof(squeal_ht_record));

        if (ht->rec[i] == NULL) {
            fprintf(stderr, "squeal_ht_init: Unable to allocate record");

            int x = 0;

            for (; x < i; x++) {
                free(ht->rec[x]);
            }

            free(ht);

            return -1;
        }

        ht->rec[i]->is_used = 0;
    }
}

int squeal_ht_add_sval(hashtable *ht, squeal_string *key, squeal_val *val)
{
    uint32_t hash;
    int slot;

    squeal_check_ht_size(ht);
    slot = squeal_ht_find_next_slot(ht);

    if (!slot) {
        return -1;
    }

    hash = squeal_ht_key_hash(key);

    ht->rec[slot]->is_used = 1;
    ht->rec[slot]->hash = hash;
    ht->rec[slot]->key = key;
    ht->rec[slot]->type = HASHTABLE_TYPE_SVAL;
    ht->rec[slot]->v.sval = val;
}

int squeal_ht_add_ptr(hashtable *ht, squeal_string *key, void *(*ptr))
{
    uint32_t hash;
    squeal_check_ht_size(ht);
    slot = squeal_ht_find_next_slot(ht);

    if (!slot) {
        return -1;
    }

    hash = squeal_ht_key_hash(key);

    ht->rec[slot]->is_used = 1;
    ht->rec[slot]->hash = hash;
    ht->rec[slot]->key = key;
    ht->rec[slot]->type = HASHTABLE_TYPE_PTR;
    ht->rec[slot]->v.ptr = &ptr;
}

squeal_val *squeal_ht_find_sval(hashtable *ht, squeal_string *key)
{
    squeal_ht_record *record = squeal_ht_find(ht, str);

    if (record == NULL
        || record->type != HASHTABLE_TYPE_SVAL
        || !record->v.sval) {
        return NULL;
    }

    return *record->v.sval;
}

squeal_val *squeal_ht_find_ptr(hashtable *ht, squeal_string *str)
{
    squeal_ht_record *record = squeal_ht_find(ht, str);

    if (record == NULL
        || record->type != HASHTABLE_TYPE_PTR
        || !record->v.ptr
        || !*record->v.ptr) {
        return NULL;
    }

    return *record->v.ptr;
}

void squeal_ht_free(hashtable *ht)
{
}

squeal_ht_record *squeal_ht_find(hashtable *ht, squeal_string *key)
{
    uint32_t hash = squeal_ht_key_hash(key);
    uint32_t slot = hash & ht->ma.mask;

    /* Direct find. Return it */
    if (ht->rec[slot]->is_used && ht->rec[slot]->hash == hash) {
        return ht->rec[slot];
    }

    /* Gonna have to iterate until we find it */
    int i = -1;
    unsigned long total_slots = ht->ma.mask + 1;
    int prev = slot - 1;
    int next = slot + 1;

    /*
     * Iterate one back, one up until we find it
     */
    while (prev > 0 || next < total_slots) {
        if (prev > 1) {
            if (ht->rec[prev]->is_used && ht->rec[prev]->hash == hash) {
                return ht->rec[prev];
            }

            prev--;
        }

        if (next < total_slots) {
            if (ht->rec[next]->is_used && ht->rec[next]->hash == hash) {
                return ht->rec[next];
            }

            next++;
        }
    }

    /* Couldn't find it */
    return NULL;
}

static uint32_t squeal_ht_key_hash(squeal_string *key)
{
    return hash(key->val, key->len, ht->seed);
}

static uint32_t squeal_ht_find_next_slot(hashtable *ht, uint32_t hash)
{
    uint32_t slot = hash & ht->ma.mask;

    /*
     * If the hashed slot is available, return it
     */
    if (!ht->rec[slot]->is_used) {
        return slot;
    }

    int i = -1;
    unsigned long total_slots = ht->ma.mask + 1;
    int prev = slot - 1;
    int next = slot + 1;

    /*
     * Iterate one back, one up until we have an open slot
     */
    while (prev > 0 || next < total_slots) {
        if (prev > 1) {
            if (!ht->rec[prev]->is_used) {
                i = prev;
                break;
            }

            prev--;
        }

        if (next < total_slots) {
            if (!ht->rec[next]->is_used) {
                i = next;
                break;
            }

            next++;
        }
    }

    /*
     * In theory, this should never happen. But I'm not the
     * smartest person in the world, so I'll just assume it could.
     */
    if (i == -1) {
        fprintf(stderr, "squeal_ht_find_next_slot: unable to find an available slot");
        return -1;
    }

    return i;
}

static int squeal_check_ht_size(hashtable *ht)
{
    double percentage = (ht->ma.filled / ((ht->ma.mask + 1) * .67));
    int alloc_stat = 1;

    if (percentage >= .67) {
        alloc_stat = squeal_realloc_ht(ht);
    }

    return alloc_stat;
}

static int squeal_realloc_ht(hashtable *ht)
{
    unsigned long actual_mask = ht->ma.mask + 1;
    unsigned long next_size = (actual_mask << 1);
    unsigned long unalloc_slots = next_size - actual_mask;
    int i;

    ht = (hashtable *) realloc(ht, sizeof(hashtable) + sizeof(squeal_ht_record) * next_size);

    if (ht == NULL) {
        fprintf(stderr, "squeal_realloc_ht: unable to realloc hashtable");
        return -1;
    }

    for (i = actual_mask; i < unalloc_slots; i++) {
        ht->rec[i] = (squeal_ht_record *) malloc(sizeof(squeal_ht_record));

        if (ht->rec[i] == NULL) {
            fprintf(stderr, "squeal_realloc_ht: unable to alloc record");

            int x = actual_mask;

            for (; x < i; x++) {
                free(ht->rec[x]);
            }

            return -1;
        }

        ht->rec[i]->is_used = 0;
    }

    return 1;
}

static long rand_seed(long max)
{
    unsigned long
            num_bins = (unsigned long) max + 1,
            num_rand = (unsigned long) RAND_MAX + 1,
            bin_size = num_rand / num_bins,
            defect   = num_rand % num_bins;

    long x;

    do {
        x = random();
    } while (num_rand - defect <= (unsigned long)x);

    return x/bin_size;
}

static uint32_t hash(const char *key, uint32_t len, uint32_t seed)
{
    static const uint32_t c1 = 0xcc9e2d51;
    static const uint32_t c2 = 0x1b873593;
    static const uint32_t r1 = 15;
    static const uint32_t r2 = 13;
    static const uint32_t m = 5;
    static const uint32_t n = 0xe6546b64;

    uint32_t hash = seed;
    const int nblocks = len / 4;
    const uint32_t *blocks = (const uint32_t *) key;
    int i;
    uint32_t k;

    for (i = 0; i < nblocks; i++) {
        k = blocks[i];
        k *= c1;
        k = ROT32(k, r1);
        k *= c2;

        hash ^= k;
        hash = ROT32(hash, r2) * m + n;
    }

    const uint8_t *tail = (const uint8_t *) (key + nblocks * 4);
    uint32_t k1 = 0;

    switch (len & 3) {
        case 3:
            k1 ^= tail[2] << 16;
        case 2:
            k1 ^= tail[1] << 8;
        case 1:
            k1 ^= tail[0];

            k1 *= c1;
            k1 = ROT32(k1, r1);
            k1 *= c2;
            hash ^= k1;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}