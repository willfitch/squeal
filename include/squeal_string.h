#ifndef HAVE_SQUEAL_STRING_H
#define HAVE_SQUEAL_STRING_H

squeal_string *squeal_string_init(const char *val, size_t len);
squeal_string *squeal_string_alloc(size_t len);
squeal_string *squeal_string_dup(squeal_string *s);
squeal_string *squeal_string_realloc(squeal_string *s, size_t newlen);

int squeal_string_equals(squeal_string *s1, squeal_string *s2);
void squeal_string_empty(squeal_string *s);
void squeal_string_free(squeal_string *str);

#endif
