#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int>  mySharedWork;
std::atomic<bool> dataProduced{false};
std::atomic<bool> dataConsume{false};

void dataProducer() {
  mySharedWork = {1, 3, 4};
  dataProduced.store(true, std::memory_order_release);
}

void delverBoy() {
  while (!dataProduced.load(std::memory_order_acquire))
    ;

  dataConsume.store(true, std::memory_order_release);
}

void dataConsumer() {
  while (!dataConsume.load(std::memory_order_acquire))
    ;
  mySharedWork[1] = 1;
}

int main(int argc, char **argv) {
  std::cout << std::endl;

  for (auto &item : mySharedWork) {
    std::cout << item << " ";
  }
  std::cout << std::endl;

  std::thread t1{dataConsumer};
  std::thread t2{delverBoy};
  std::thread t3{dataProducer};

  t1.join();
  t2.join();
  t3.join();

  for (auto &item : mySharedWork) {
    std::cout << item << " ";
  }
  std::cout << std::endl;
}
