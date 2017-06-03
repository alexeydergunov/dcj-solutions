// Sample input 6, in CPP.
#include <cassert>
#include <stdlib.h>
#include "message.h"

static int isthenodebroken = 0;

long long GetLength() {
  return 1LL;
}

long long GetValue(long long i) {
  if (i < 0LL || i >= GetLength())
    assert(0);
  int val = (isthenodebroken ? (rand() % 2) : 1);
  if (i == 0)
    isthenodebroken = 1;
  return val;
}
