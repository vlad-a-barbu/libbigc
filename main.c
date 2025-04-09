#include "big.h"

void test_bigint_init() {
  BigInt x = bigint_init("123");
  assert(x.ndigits == 3);
  assert(streq("123", x.str));
  bigint_deinit(&x);
}

void test_bigint_add() {
  BigInt x = bigint_init("1789");
  BigInt y = bigint_init("3456");
  BigInt z = bigint_add(&x, &y);
  assert(streq("5245", z.str));
  bigint_deinit(&x);
  bigint_deinit(&y);
  bigint_deinit(&z);
}

int main() {
  test_bigint_init();
  test_bigint_add();
  return(0);
}
