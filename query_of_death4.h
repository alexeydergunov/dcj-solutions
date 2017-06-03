// Sample input 4, in CPP.
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
  int val = (isthenodebroken ? 1 : 0);
  if (i == 15)
    isthenodebroken = 1;
  return val;
}
