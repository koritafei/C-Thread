#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex_;

std::condition_variable condVar;

bool dataReady{false};

void doTheWork() {
  std::cout << "Processing the shared data" << std::endl;
}

void waitngForWork() {
  std::cout << "Worker: Waiting for work." << std::endl;
  std::unique_lock<std::mutex> lck{mutex_};

  condVar.wait(lck, [] {
    return dataReady;
  });

  doTheWork();
  std::cout << " done work " << std::endl;
}

void setDataReady() {
  {
    std::lock_guard<std::mutex> lck{mutex_};
    dataReady = true;
  }

  condVar.notify_one();
}

int main() {
  std::thread t1{setDataReady};
  std::thread t2{waitngForWork};

  t1.join();
  t2.join();
}