// Sample input 3, in CPP.

#include <cassert>
#include <string>

const std::string __s__ = "((()(";

long long GetLength() {
  return (long long) (__s__.size());
}

char GetCharacter(long long index) {

  int i = (int)index;
  if (i >= 0 && i < (int)__s__.size()) {
    return __s__[i];
  } else {
    assert(0);
  }
}
