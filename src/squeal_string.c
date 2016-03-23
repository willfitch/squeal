#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <squeal_types.h>
#include <squeal_string.h>


squeal_string *squeal_string_init(const char *val, size_t len)
{
    squeal_string *retval = squeal_string_alloc(len);
    strncpy(retval->val, val, len);
    return retval;
}

squeal_string *squeal_string_alloc(size_t len)
{
    squeal_string *retval;
    retval = (squeal_string *) malloc(sizeof(squeal_string) + sizeof(char) * len);
    retval->len = len;
    memset(retval->val, '\0', len);
    return retval;
}

squeal_string *squeal_string_dup(squeal_string *s)
{
    return squeal_string_init(s->val, s->len);
}

squeal_string *squeal_string_realloc(squeal_string *s, size_t newlen)
{
    squeal_string *retval;
    retval = (squeal_string *) realloc(s, newlen);
    retval->len = newlen;
    return retval;
}

void squeal_string_empty(squeal_string *s)
{
    memset(s->val, '\0', s->len);
}

int squeal_string_equals(squeal_string *s1, squeal_string *s2)
{
    return s1 == s2 || (s1->len == s2->len && !memcmp(s1->val, s2->val, s1->len));
}

void squeal_string_free(squeal_string *str)
{
    if (str) {
        free(str);
    }
}
