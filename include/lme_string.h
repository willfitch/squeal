#ifndef HAVE_LME_STRING_H
#define HAVE_LME_STRING_H

lme_string *lme_string_init(const char *val, size_t len);
lme_string *lme_string_alloc(size_t len);
lme_string *lme_string_dup(lme_string *s);
lme_string *lme_string_realloc(lme_string *s, size_t newlen);
int lme_string_equals(lme_string *s1, lme_string *s2);
void lme_string_empty(lme_string *s);
void lme_string_free(lme_string *str);

#endif
