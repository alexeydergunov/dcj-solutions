// Sample input 3, in CPP.

#include <cassert>

long long GetN() {
  return 1LL;
}

char GetFavoriteMove(long long id) {
  switch ((int)id) {
    case 0: return 'P';
    case 1: return 'S';
    default: assert(0);
  }
}
