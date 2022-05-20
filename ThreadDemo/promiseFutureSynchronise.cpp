#include <future>
#include <iostream>
#include <thread>
#include <utility>

void doTheWork() {
  std::cout << "Processing shared data" << std::endl;
}

void waitForWork(std::future<void> &&fut) {
  std::cout << "Worker: waiting for work" << std::endl;
  fut.wait();
  doTheWork();

  std::cout << "work done" << std::endl;
}

void setDataReady(std::promise<void> &&prom) {
  std::cout << "Data is ready" << std::endl;
  prom.set_value();
}

int main() {
  std::cout << std::endl;
  std::promise<void> sendReady;

  auto        fut = sendReady.get_future();
  std::thread t1{waitForWork, std::move(fut)};

  std::thread t2{setDataReady, std::move(sendReady)};

  t1.join();
  t2.join();
}
