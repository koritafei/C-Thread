#include <chrono>
#include <iostream>
#include <map>
#include <shared_mutex>
#include <string>
#include <thread>

std::map<std::string, unsigned long long> teleBook{{"huyun", 188888888},
                                                   {"qw", 122288282},
                                                   {"as", 1000000}};

std::shared_timed_mutex teleBookMutex;

void addToTeleBook(const std::string &name, unsigned long long number) {
  std::lock_guard<std::shared_timed_mutex> writerLock{teleBookMutex};
  std::cout << "\nTRATING UPDATE " << name;
  std::this_thread::sleep_for(std::chrono::microseconds(500));
  teleBook[name] = number;
  std::cout << " ... ENDING UPDATE" << std::endl;
}

void printNumber(const std::string &name) {
  std::shared_lock<std::shared_timed_mutex> readerLock{teleBookMutex};
  if (teleBook.find(name) == teleBook.end()) {
    std::cout << name << " not found from telebook." << std::endl;
  } else {
    std::cout << name << " : " << teleBook[name] << std::endl;
  }
}

int main() {
  std::thread reader1{[] {
    printNumber("qw");
  }};

  std::thread reader2{[] {
    printNumber("huyun");
  }};

  std::thread w1{[] {
    addToTeleBook("qw", 11111111111);
  }};

  std::thread reader3{[] {
    printNumber("qw");
  }};

  std::thread reader4{[] {
    printNumber("huyun");
  }};

  std::thread w2{[] {
    addToTeleBook("rt", 2222222222);
  }};

  std::thread reader5{[] {
    printNumber("qw");
  }};

  std::thread reader6{[] {
    printNumber("huyun");
  }};

  std::thread reader7{[] {
    printNumber("rt");
  }};

  std::thread reader8{[] {
    printNumber("aaaa");
  }};

  reader1.join();
  reader2.join();
  reader3.join();
  reader4.join();
  reader5.join();
  reader6.join();
  reader7.join();
  reader8.join();
  w1.join();
  w2.join();

  for (auto teleIt : teleBook) {
    std::cout << teleIt.first << ": " << teleIt.second << std::endl;
  }
}