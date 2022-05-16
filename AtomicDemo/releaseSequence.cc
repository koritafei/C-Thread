#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

std::atomic<int> atom{0};
int              someThingShared{0};

using namespace std::chrono_literals;

void wirteShared() {
  someThingShared = 2;
  atom.store(2, std::memory_order_release);
}

void readShared() {
  while (!(atom.fetch_sub(1, std::memory_order_acquire) > 0)) {
    std::this_thread::sleep_for(100ms);
  }

  std::cout << "someThingShared: " << someThingShared << std::endl;
}

int main(int argc, char **argv) {
  std::cout << std::endl;

  std::thread t1{wirteShared};
  std::thread t2{readShared};
  std::thread t3{readShared};

  t1.join();
  t2.join();
  t3.join();

  std::cout << "atom: " << atom << std::endl;

  std::cout << std::endl;
}