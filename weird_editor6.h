// Sample input 6, in CPP.

#include <cassert>

long long GetNumberLength() {
  return 6LL;
}

long long GetDigit(long long i) {
  switch ((int)i) {
    case 0: return 9LL;
    case 1: return 8LL;
    case 2: return 9LL;
    case 3: return 7LL;
    case 4: return 8LL;
    case 5: return 6LL;
    default: assert(0);
  }
}
