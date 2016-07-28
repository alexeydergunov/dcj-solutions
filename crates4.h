// Sample input 3, in CPP.

#include <cassert>

long long GetNumStacks() {
  return 4LL;
}

long long GetStackHeight(long long i) {
  switch ((int)i) {
    case 1: return 10LL;
    case 2: return 10LL;
    case 3: return 0LL;
    case 4: return 0LL;
    default: assert(0);
  }
}
