#include <atomic>
#include <iostream>

template <typename T> T fetch_multi(std::atomic<T> &shared, T multi) {
  T oldvalue = shared.load();
  while (!shared.compare_exchange_strong(oldvalue, oldvalue * multi))
    ;
  return oldvalue;
}

int main() {
  std::atomic<int> myInt{5};
  std::cout << myInt << std::endl;
  fetch_multi(myInt, 5);
  std::cout << myInt << std::endl;
}