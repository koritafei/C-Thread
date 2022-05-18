#include <future>
#include <iostream>
#include <thread>

int main() {
  int         res;
  std::thread t{[&] {
    res = 2000 + 11;
  }};

  t.join();

  std::cout << "res : " << res << std::endl;

  auto fut = std::async([] {
    return 2000 + 11;
  });

  std::cout << "fut.get() " << fut.get() << std::endl;
}
