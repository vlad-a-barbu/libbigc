#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct {
  char    *buff;
  size_t  len;
  size_t  cap;
} DynArray;

DynArray dynarray_init(size_t cap) {
  assert(cap > 0 && "invalid capacity");
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
    assert(darr->cap > 0 && "invalid capacity");
    char *mem = (char *)realloc(darr->buff, darr->cap);
    assert(mem && "malloc failed");
    darr->buff = mem;
  }
  darr->buff[darr->len++] = val;
}

size_t strlen(const char *str) {
  size_t len = 0;
  for (; *str != '\0'; ++str, ++len) {}
  return(len);
}

int streq(const char *str1, const char *str2) {
  for (; *str1 != '\0' && *str2 != '\0'; ++str1, ++str2) {
    if (*str1 != *str2) return(0);
  }
  return(*str1 == '\0' && *str2 == '\0');
}

void swap(void **x, void **y) {
  void *tmp = *x;
  *x = *y;
  *y = tmp;
}
