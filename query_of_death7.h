// Sample input 7, in CPP.
#include <cassert>
#include <stdlib.h>
#include "message.h"

static int isthenodebroken = 0;

long long GetLength() {
  return 10010000;
}

long long GetValue(long long i) {
  if (i < 0LL || i >= GetLength())
    assert(0);
  int val = (isthenodebroken ? (rand() % 2) : (i % 7 == 0 ? 1 : 0));
  if (i == 4567891)
    isthenodebroken = 1;
  return val;
}

// answer must be 1430000
