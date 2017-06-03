// Sample input 5, in CPP.
#include <cassert>
#include <stdlib.h>
#include "message.h"

static int isthenodebroken = 0;

long long GetLength() {
  return 100LL;
}

long long GetValue(long long i) {
  if (i < 0LL || i >= GetLength())
    assert(0);
  int val = (isthenodebroken ? (rand() % 2) : 1);
  if (i == 28)
    isthenodebroken = 1;
  return val;
}
