/**
 * @file spinlock.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief std::atomic_falg 实现回旋互斥锁
 * @version 0.1
 * @date 2020-07-17
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <unistd.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

class spinlock_mutex {
public:
  spinlock_mutex() : flag_(ATOMIC_FLAG_INIT) {}

  void lock() {
      while (flag_.test_and_set(std::memory_order_acquire)) {
        std::cout << "waiting lock " << std::endl;
      }
  }

  void unlock() {
    flag_.clear(std::memory_order_release);
    std::cout << "unlock" << std::endl;
  }

private:
  std::atomic_flag flag_;
};

void run(std::shared_ptr<spinlock_mutex> &ptr) {
  ptr->lock();
  sleep(1);
  ptr->unlock();
}

int main(int argc, char **argv) {
  std::shared_ptr<spinlock_mutex> ptr = std::make_shared<spinlock_mutex>();
  std::thread                     t1{run, std::ref(ptr)};
  std::thread                     t2{run, std::ref(ptr)};
  t1.join();
  t2.join();
}
