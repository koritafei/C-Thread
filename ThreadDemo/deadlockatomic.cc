#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct Ciritialata {
  std::mutex mutex_;
};

void deadLock(Ciritialata &a, Ciritialata &b) {
  std::lock(a.mutex_, b.mutex_);
  std::lock_guard<std::mutex> guard1{a.mutex_, std::adopt_lock};
  std::lock_guard<std::mutex> guard2{b.mutex_, std::adopt_lock};
  std::cout << "Thread: " << std::this_thread::get_id() << " first mutex. "
            << std::endl;

  std::this_thread::sleep_for(std::chrono::microseconds(1));
  std::cout << "Thread: " << std::this_thread::get_id() << " second mutex. "
            << std::endl;
  std::cout << "Thread: " << std::this_thread::get_id() << " both mutex. "
            << std::endl;
}

int main() {
  Ciritialata c1;
  Ciritialata c2;

  std::thread t1{[&] {
    deadLock(c1, c2);
  }};
  std::thread t2{[&] {
    deadLock(c2, c1);
  }};

  t1.join();
  t2.join();
}
