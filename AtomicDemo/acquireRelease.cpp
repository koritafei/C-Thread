#include <atomic>
#include <iostream>
#include <string>
#include <thread>

std::atomic<std::string *> ptr;
int                        data;
std::atomic<int>           atoData;

void producer() {
  std::string *p = new std::string("c++11");
  data           = 2011;
  atoData.store(2014, std::memory_order_relaxed);
  ptr.store(p, std::memory_order_release);
}

void consumer() {
  std::string *p;
  while (!(p = ptr.load(std::memory_order_acquire)))
    ;

  std::cout << "*p = " << *p << std::endl;
  std::cout << "data = " << data << std::endl;
  std::cout << "atoData = " << atoData.load(std::memory_order_relaxed)
            << std::endl;
}

int main() {
  std::thread t1{consumer};
  std::thread t2{producer};

  t1.join();
  t2.join();
}
