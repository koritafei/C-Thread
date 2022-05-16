#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

std::string work;
std::atomic<bool> ready{false};

void consumer() {
  while (!ready.load()) {
  }

  std::cout << work << std::endl;
}

void product() {
  work = "done";
  ready = true;
}

int main(int argc, char **argv) {
  std::thread proc{product};
  std::thread con{consumer};

  proc.join();
  con.join();
}
