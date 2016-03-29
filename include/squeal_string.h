#ifndef HAVE_SQUEAL_STRING_H
#define HAVE_SQUEAL_STRING_H

struct _squeal_string {
    size_t len;

    /* must be the last element! */
    char val[1];
};

SquealString *squeal_string_init(const char *val, size_t len);
SquealString *squeal_string_alloc(size_t len);
SquealString *squeal_string_dup(SquealString *s);
SquealString *squeal_string_realloc(SquealString *s, size_t newlen);

int squeal_string_equals(SquealString *s1, SquealString *s2);
void squeal_string_empty(SquealString *s);
void squeal_string_free(SquealString *str);

#endif
