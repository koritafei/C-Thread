#include <atomic>
#include <cassert>
#include <iostream>

int main(int argc, char **argv) {

  int intArray[5];
  std::atomic<int *> p(intArray);
  p++;
  assert(p.load() == &intArray[1]);
  p += 1;
  assert(p.load() == &intArray[2]);
  p -= 1;
  assert(p.load() != &intArray[1]);
}
