#include "common.h"

#define BIGINT_INIT_NDIGITS 21

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
  size_t pi = i / 2;
  assert(pi < big->digits.len);
  int pack = 0xFF & big->digits.buff[pi];
  return(i % 2 == 0 ? pack >> 4 : pack & 0xF);
}

const char *bigint_string(const BigInt *big) {
  char *mem = (char *)malloc(big->ndigits + 1);
  assert(mem && "malloc failed");
  size_t i = big->ndigits - 1;
  size_t len = 0;

  do {
    mem[len++] = bigint_ith_digit(big, i) + '0';
  } while (i-- != 0);
  mem[len] = '\0';

  return(mem);
}

BigInt bigint_init() {
  BigInt res = {0};
  res.digits = dynarray_init(BIGINT_INIT_NDIGITS);
  return res;
}

void bigint_deinit(BigInt *big) {
  assert(big && "invalid bigint");
  dynarray_deinit(&big->digits);
  big->ndigits = 0;
  free((void *)big->str);
  big->str = NULL;
}

BigInt bigint_i(int val) {
  BigInt res = bigint_init();
  int pack = 0;
  int pack_len = 0;

  while (val != 0) {
    bigint_append_digit(&res, val % 10, &pack, &pack_len);
    val /= 10;
  }

  bigint_flush_pack(&res, &pack, &pack_len);
  res.str = bigint_string(&res);

  return(res);
}

BigInt bigint_s(const char *str) {
  assert(str && "nullptr");
  size_t slen = strlen(str);
  assert(slen > 0);
  BigInt res = bigint_init();
  int pack = 0;
  int pack_len = 0;
  size_t i = slen - 1;

  do {
    bigint_append_digit(&res, str[i] - '0', &pack, &pack_len);
  } while (i-- != 0);

  bigint_flush_pack(&res, &pack, &pack_len);
  res.str = bigint_string(&res);

  return(res);
}

BigInt bigint_add(const BigInt *x, const BigInt *y) {
  BigInt res = bigint_init();
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
    if (carry) { s++; carry--; }
    carry += (s / 10);
    bigint_append_digit(&res, (s % 10), &pack, &pack_len);
  }

  if (carry) { 
    bigint_append_digit(&res, carry, &pack, &pack_len);
  }
  bigint_flush_pack(&res, &pack, &pack_len);
  res.str = bigint_string(&res);

  return(res);
}

BigInt bigint_mul(const BigInt *x, const BigInt *y) {
  BigInt res = bigint_init();
  if (x->ndigits < y->ndigits) {
    swap((void **)&x, (void **)&y);
  }

  for (size_t j = 0; j < y->ndigits; ++j) {
    BigInt tmp = bigint_init();
    int pack = 0;
    int pack_len = 0;
    int carry = 0;
    for (size_t z = 0; z < j; ++z) {
      bigint_append_digit(&tmp, 0, &pack, &pack_len);
    }
    for (size_t i = 0; i < x->ndigits; ++i) {
      int p = bigint_ith_digit(x, i) * bigint_ith_digit(y, j);
      if (carry) {  
        p += carry;
        carry = 0;
      }
      carry += (p / 10);
      bigint_append_digit(&tmp, (p % 10), &pack, &pack_len);
    }
    if (carry) {
      bigint_append_digit(&tmp, carry, &pack, &pack_len);
    }
    bigint_flush_pack(&tmp, &pack, &pack_len);

    BigInt old = res;
    res = bigint_add(&res, &tmp);

    bigint_deinit(&old);
    bigint_deinit(&tmp);
  }

  return(res);
}
