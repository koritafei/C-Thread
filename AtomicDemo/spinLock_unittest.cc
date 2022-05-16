#include <atomic>
#include <iostream>
#include <thread>

class SpinLock {

public:
  void lock() {
    std::cout << "lock" << std::endl;
    while (atomic_flag.test_and_set())
      ;
  }

  void unlock() {
    std::cout << "unlock" << std::endl;
    atomic_flag.clear();
  }

private:
  std::atomic_flag atomic_flag = ATOMIC_FLAG_INIT;
};

SpinLock spin;

void workOnResource() {
  spin.lock();
  spin.unlock();
}

int main() {
  std::thread t1(workOnResource);
  std::thread t2(workOnResource);

  t1.join();
  t2.join();
}
