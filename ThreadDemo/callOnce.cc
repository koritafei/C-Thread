#include <iostream>
#include <mutex>
#include <thread>

std::once_flag onceFlag;

void doOnce() {
  std::call_once(onceFlag, [] {
    std::cout << "do once" << std::endl;
  });
}

void doOnce1() {
  std::call_once(onceFlag, [] {
    std::cout << "do once 2" << std::endl;
  });
}

int main() {
  std::thread t1{doOnce};
  std::thread t2{doOnce};
  std::thread t3{doOnce1};
  std::thread t4{doOnce1};

  t1.join();
  t2.join();
  t3.join();
  t4.join();
}