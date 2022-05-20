#include <functional>
#include <future>
#include <iostream>
#include <utility>
#include <vector>

void calcProducts(std::packaged_task<int(int, int)> &     task,
                  const std::vector<std::pair<int, int>> &pairs) {
  for (auto &pair : pairs) {
    auto future = task.get_future();
    task(pair.first, pair.second);

    std::cout << pair.first << " * " << pair.second << " = " << future.get()
              << std::endl;
    task.reset();
  }
}

int main() {
  std::vector<std::pair<int, int>> allPairs;
  allPairs.push_back(std::make_pair(1, 2));
  allPairs.push_back(std::make_pair(3, 4));
  allPairs.push_back(std::make_pair(5, 8));
  allPairs.push_back(std::make_pair(6, 9));

  std::packaged_task<int(int, int)> task{[](int fir, int sec) {
    return fir * sec;
  }};

  calcProducts(task, allPairs);

  std::cout << "\n\n";

  std::thread t{calcProducts, std::ref(task), allPairs};

  t.join();
}