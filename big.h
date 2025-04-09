#include "helpers.h"

#define BIGINT_INIT_NDIGITS 20

typedef struct {
  DynArray   digits;
  size_t     ndigits;
  const char *str;
} BigInt;

void bigint_flush_pack(BigInt *big, int *pack_out, int *pack_len_out) {
  int pack = *pack_out;
  int pack_len = *pack_len_out;

  if (pack_len == 0) return;
  if (pack_len == 1) pack <<= 4;

  dynarray_append(&big->digits, pack);
  big->ndigits += pack_len;

  *pack_out = 0;
  *pack_len_out = 0;
}

void bigint_append_digit(BigInt *big, int dig, int *pack_out, int *pack_len_out) {
  assert(dig >= 0 && dig <= 9 && "invalid digit");
  int pack = *pack_out;
  int pack_len = *pack_len_out;

  if (pack_len++ == 1) {
    pack = (pack << 4) | dig;
  } else {
    pack = dig;
  }

  if (pack_len == 2) {
    bigint_flush_pack(big, &pack, &pack_len);
  }

  *pack_out = pack;
  *pack_len_out = pack_len;
}

int bigint_ith_digit(const BigInt *big, size_t i) {
  assert(i < big->ndigits);
  size_t j = i / 2;
  assert(j < big->digits.len);
  int pack = 0xFF & big->digits.buff[i / 2];
  if (i % 2 == 0) {
    return(pack >> 4);
  }
  return(pack & 0xF);
}

const char *bigint_string(const BigInt *big) {
  char *mem = (char *)malloc(big->ndigits + 1);
  assert(mem && "malloc failed");
  size_t i = big->ndigits - 1;
  size_t len = 0;

  do {
    int d = bigint_ith_digit(big, i);
    mem[len++] = d + '0';
  } while (i-- != 0);
  mem[len] = '\0';

  return(mem);
}

BigInt bigint_init(size_t cap) {
  BigInt res = {0};
  res.digits = dynarray_init(cap);
  return res;
}

BigInt bigint_fromstr(const char *str) {
  BigInt res = bigint_init(BIGINT_INIT_NDIGITS);
  size_t slen = strlen(str);
  assert(slen > 0);
  int pack = 0;
  int pack_len = 0;
  size_t i = slen - 1;

  do {
    int dig = str[i] - '0';
    bigint_append_digit(&res, dig, &pack, &pack_len);
  } while (i-- != 0);

  bigint_flush_pack(&res, &pack, &pack_len);
  res.str = bigint_string(&res);

  return(res);
}

BigInt bigint_add(const BigInt *x, const BigInt *y) {
  BigInt res = bigint_init(BIGINT_INIT_NDIGITS);
  int pack = 0;
  int pack_len = 0;
  int carry = 0;

  if (x->ndigits < y->ndigits) {
    swap((void **)&x, (void **)&y);
  }

  for (size_t i = 0; i < x->ndigits; ++i) {
    int s = i < y->ndigits
      ? bigint_ith_digit(x, i) + bigint_ith_digit(y, i)
      : bigint_ith_digit(x, i);
    if (carry) { 
      assert(carry == 1);
      s++; carry--;
    }
    carry += (s / 10);
    bigint_append_digit(&res, (s % 10), &pack, &pack_len);
  }

  if (carry) {
    assert(carry == 1);
    bigint_append_digit(&res, carry, &pack, &pack_len);
  }
  bigint_flush_pack(&res, &pack, &pack_len);
  res.str = bigint_string(&res);

  return(res);
}

void bigint_deinit(BigInt *big) {
  assert(big && "invalid bigint");
  dynarray_deinit(&big->digits);
  big->ndigits = 0;
  free((void *)big->str);
  big->str = NULL;
}
