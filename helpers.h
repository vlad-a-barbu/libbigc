#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

size_t strlen(const char *str) {
  size_t len = 0;
  for (; *str != '\0'; ++str, ++len) {}
  return(len);
}

int streq(const char *s1, const char *s2) {
  for (; *s1 != '\0' && *s2 != '\0'; ++s1, ++s2) {
    if (*s1 != *s2) return(0);
  }
  return(*s1 == '\0' && *s2 == '\0');
}

typedef struct {
  char    *buff;
  size_t  len;
  size_t  cap;
} DynArray;

DynArray dynarray_init(size_t cap) {
  DynArray res = {0};
  res.cap = cap;
  char *mem = (char *)malloc(cap);
  assert(mem && "malloc failed");
  res.buff = mem;
  return(res);
}

void dynarray_deinit(DynArray *darr) {
  assert(darr && "invalid dynarray");
  free(darr->buff);
  darr->buff = NULL;
  darr->len = 0;
  darr->cap = 0;
}

void dynarray_append(DynArray *darr, char val) {
  if (darr->len == darr->cap) {
    darr->cap *= 2;
    char *mem = (char *)realloc(darr->buff, darr->cap);
    assert(mem && "malloc failed");
    darr->buff = mem;
  }
  darr->buff[darr->len++] = val;
}
