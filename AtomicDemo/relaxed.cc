#include <atomic>
#include <iostream>
#include <thread>
#include <vector>

std::atomic<int> count = {0};

void add() {
  for (int i = 0; i < 1000; i++) {
    count.fetch_add(1, std::memory_order_relaxed);
  }
}

int main() {
  std::vector<std::thread> v;

  for (int i = 0; i < 10; i++) {
    v.emplace_back(add);
  }

  for (auto &t : v) {
    t.join();
  }

  std::cout << count << std::endl;
}
