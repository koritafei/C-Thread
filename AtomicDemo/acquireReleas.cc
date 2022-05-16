#include <atomic>
#include <iostream>
#include <string>
#include <thread>

using namespace std;

atomic<string *> ptr;
int              pdata;
atomic<int>      atoData;

void produnctor() {
  string *p = new string("c++11");
  pdata     = 2011;
  atoData.store(2014, std::memory_order_relaxed);
  ptr.store(p, std::memory_order_release);
}

void consumer() {
  string *p2;
  while (!(p2 = ptr.load(std::memory_order_acquire)))
    ;

  cout << "*p2: " << *p2 << endl;
  cout << "data: " << pdata << endl;
  cout << "atoData: " << atoData.load(std::memory_order_relaxed) << endl;
}

int main() {
  std::thread t1{produnctor};
  std::thread t2{consumer};

  t1.join();
  t2.join();

  std::cout << endl;
}