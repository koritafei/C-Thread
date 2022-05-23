#include <algorithm>
#include <execution>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

std::mutex m;

int main() {
  std::vector<int> v   = {1, 2, 3};
  int              sum = 0;

  std::for_each(std::execution::par_unseq, v.begin(), v.end(), [&sum](int i) {
    std::lock_guard<std::mutex> lock{m};
    sum += i + i;
    std::cout << "ThreadId : " << std::this_thread::get_id() << " : " << sum
              << std::endl;
  });
}
