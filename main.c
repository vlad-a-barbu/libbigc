#include "big.h"

void test_bigint_init() {
  BigInt x = bigint_s("123");
  assert(x.ndigits == 3);
  assert(streq("123", x.str));
  bigint_deinit(&x);
}

void test_bigint_add(const char *xstr, const char *ystr, const char *zstr) {
  BigInt x = bigint_s(xstr);
  BigInt y = bigint_s(ystr);
  BigInt z = bigint_add(&x, &y);
  assert(streq(zstr, z.str));
  bigint_deinit(&x);
  bigint_deinit(&y);
  bigint_deinit(&z);
}

void test_bigint_mul(const char *xstr, const char *ystr, const char *zstr) {
  BigInt x = bigint_s(xstr);
  BigInt y = bigint_s(ystr);
  BigInt z = bigint_mul(&x, &y);
  assert(streq(zstr, z.str));
  bigint_deinit(&x);
  bigint_deinit(&y);
  bigint_deinit(&z);
}

int main() {
  test_bigint_init();
  test_bigint_add("17", "2", "19");
  test_bigint_add("3", "17", "20");
  test_bigint_add("1789", "3456", "5245");
  test_bigint_add(
    "132391239129319392131392193219391391293119312",
       "999813821893219838912398213982139821398121",
    "133391052951212611970304591433373531114517433");
  test_bigint_mul("3", "9", "27");
  test_bigint_mul(
    "31231231231213123128132921312313198",
    "91231282198123891298128321113913012",
    "2849265269849664699762995815417216184024643963530951348925147071532376");
  return(0);
}
