#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <squeal_types.h>
#include <squeal_string.h>


SquealString *squeal_string_init(const char *val, size_t len)
{
    SquealString *retval = squeal_string_alloc(len);
    strncpy(retval->val, val, len);
    return retval;
}

SquealString *squeal_string_alloc(size_t len)
{
    SquealString *retval;
    retval = (SquealString *) malloc(sizeof(SquealString) + sizeof(char) * len);
    retval->len = len;
    memset(retval->val, '\0', len);
    return retval;
}

SquealString *squeal_string_dup(SquealString *s)
{
    return squeal_string_init(s->val, s->len);
}

SquealString *squeal_string_realloc(SquealString *s, size_t newlen)
{
    SquealString *retval;
    retval = (SquealString *) realloc(s, newlen);
    retval->len = newlen;
    return retval;
}

void squeal_string_empty(SquealString *s)
{
    memset(s->val, '\0', s->len);
}

int squeal_string_equals(SquealString *s1, SquealString *s2)
{
    return s1 == s2 || (s1->len == s2->len && !memcmp(s1->val, s2->val, s1->len));
}

void squeal_string_free(SquealString *str)
{
    if (str) {
        free(str);
    }
}
