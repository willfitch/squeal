#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lme_types.h"
#include "lme_string.h"


lme_string *lme_string_init(const char *val, size_t len)
{
    lme_string *retval = lme_string_alloc(len);
    strncpy(retval->val, val, len);
    return retval;
}

lme_string *lme_string_alloc(size_t len)
{
    lme_string *retval;
    retval = (lme_string *) malloc(sizeof(lme_string) + sizeof(char) * len);
    retval->len = len;
    memset(retval->val, '\0', len);
    return retval;
}

lme_string *lme_string_dup(lme_string *s)
{
    return lme_string_init(s->val, s->len);
}

lme_string *lme_string_realloc(lme_string *s, size_t newlen)
{
    lme_string *retval;
    retval = (lme_string *) realloc(s, newlen);
    retval->len = newlen;
    return retval;
}

void lme_string_empty(lme_string *s)
{
    memset(s->val, '\0', s->len);
}

int lme_string_equals(lme_string *s1, lme_string *s2)
{
    return s1 == s2 || (s1->len == s2->len && !memcmp(s1->val, s2->val, s1->len));
}

void lme_string_free(lme_string *str)
{
    if (str) {
        free(str);
    }
}
