// Sample input 5, in CPP.

#include <cassert>

long long GetStackSize() {
  return 8LL;
}

long long GetNumDiners() {
  return 8LL;
}

long long GetStackItem(long long i) {
  switch ((int)i) {
    case 0: return 7LL;
    case 1: return 6LL;
    case 2: return 5LL;
    case 3: return 5LL;
    case 4: return 5LL;
    case 5: return 5LL;
    case 6: return 1LL;
    case 7: return 0LL;
    default: assert(0);
  }
}
