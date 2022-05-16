#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>

class SpinLock {
public:
  void lock() {
    std::cout << "lock" << std::endl;
    while (flag.test_and_set())
      ;
  }

  void unlock() {
    std::cout << "unlock" << std::endl;
    flag.clear();
  }

private:
  std::atomic_flag flag = ATOMIC_FLAG_INIT;
};

SpinLock spin;

void workOnResource() {
  spin.lock();
  std::this_thread::sleep_for(std::chrono::microseconds(2000));
  spin.unlock();
}

int main() {
  std::thread t1(workOnResource);
  std::thread t2(workOnResource);

  t1.join();
  t2.join();
}
