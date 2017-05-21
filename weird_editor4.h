// Sample input 4, in CPP.

#include <cassert>

long long GetNumberLength() {
  return 20LL;
}

long long GetDigit(long long i) {
  switch ((int)i) {
    case 0: return 5LL;
    case 1: return 5LL;
    case 2: return 5LL;
    case 3: return 5LL;
    case 4: return 5LL;
    case 5: return 4LL;
    case 6: return 4LL;
    case 7: return 4LL;
    case 8: return 4LL;
    case 9: return 4LL;
    case 10: return 3LL;
    case 11: return 3LL;
    case 12: return 3LL;
    case 13: return 3LL;
    case 14: return 3LL;
    case 15: return 2LL;
    case 16: return 2LL;
    case 17: return 2LL;
    case 18: return 2LL;
    case 19: return 2LL;
    default: assert(0);
  }
}
