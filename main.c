#include "big.h"

void test_bigint_init() {
  BigInt x = bigint_init("123");
  assert(x.ndigits == 3);
  assert(streq("123", x.str));
  bigint_deinit(&x);
}

void test_bigint_add(const char *xstr, const char *ystr, const char *zstr) {
  BigInt x = bigint_init(xstr);
  BigInt y = bigint_init(ystr);
  BigInt z = bigint_add(&x, &y);
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
  return(0);
}
