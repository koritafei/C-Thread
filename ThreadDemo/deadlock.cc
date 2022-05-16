#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

struct Ciritialata {
  std::mutex mutex_;
};

void deadLock(Ciritialata &a, Ciritialata &b) {
  std::unique_lock<std::mutex> guard1{a.mutex_, std::defer_lock};

  std::cout << "Thread: " << std::this_thread::get_id() << " first mutex. "
            << std::endl;

  std::this_thread::sleep_for(std::chrono::microseconds(1));
  std::unique_lock<std::mutex> guard2{b.mutex_, std::defer_lock};
  std::cout << "Thread: " << std::this_thread::get_id() << " second mutex. "
            << std::endl;
  std::cout << "Thread: " << std::this_thread::get_id() << " both mutex. "
            << std::endl;

  lock(guard1, guard2);
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
