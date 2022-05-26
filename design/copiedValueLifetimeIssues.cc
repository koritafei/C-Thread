#include <chrono>
#include <iostream>
#include <string>
#include <thread>

void executeTwoThread() {
  const std::string localString{"local string"};

  std::thread t1{[localString] {
    std::cout << "Per Copy " << localString << std::endl;
  }};

  std::thread t2{[&localString] {
    std::cout << "Per Ref " << localString << std::endl;
  }};

  t1.detach();
  t2.detach();
}

using namespace std::chrono_literals;

int main() {
  executeTwoThread();
  std::this_thread::sleep_for(1s);
}
