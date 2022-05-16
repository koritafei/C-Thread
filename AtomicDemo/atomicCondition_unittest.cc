#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

std::vector<int> mySharedWork;
std::atomic<bool> dataReadly{false};

void waitingForWork() {
  std::cout << "waiting" << std::endl;
  while (!dataReadly.load()) {
    std::this_thread::sleep_for(std::chrono::microseconds(5));
  }
  mySharedWork[1] = 2;
  std::cout << "work done" << std::endl;
}

void setDataReady() {
  mySharedWork = {1, 0, 3};
  dataReadly = true;
  std::cout << "Data Prepared" << std::endl;
}

int main() {
  std::cout << std::endl;

  std::thread t1{waitingForWork};
  std::thread t2{setDataReady};

  t1.join();
  t2.join();

  for (auto &it : mySharedWork) {
    std::cout << it << ' ';
  }

  std::cout << std::endl;
}