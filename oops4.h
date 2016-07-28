// Sample input 3, in CPP.

#include <cassert>

typedef long long ll;

long long GetN() {
  return 2LL;
}

long long GetNumber(long long i) {
  switch ((int)i) {
    case 0: return (ll) 1e18;
    case 1: return (ll) -1e18;
    default: assert(0);
  }
}
