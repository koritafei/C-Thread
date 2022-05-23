#include <atomic>
#include <chrono>
#include <iostream>
#include <random>
#include <vector>

constexpr long long size = 10000000;

int main() {
  std::vector<int> randValues;
  randValues.reserve(size);

  std::random_device              seed;
  std::mt19937                    engine(seed());
  std::uniform_int_distribution<> uniformDIst(1, 10);

  for (long long i = 0; i < size; i++) {
    randValues.push_back(uniformDIst(engine));
  }

  const auto sta = std::chrono::steady_clock::now();

  std::atomic<unsigned long long> sum{};
  std::cout << std::boolalpha << "sum.is_lock_free() : " << sum.is_lock_free()
            << std::endl;
  for (auto &n : randValues) {
    sum += n;
  }

  std::chrono::duration<double> dur = std::chrono::steady_clock::now() - sta;

  std::cout << "Time for mySumition " << dur.count() << "seconds" << std::endl;
  std::cout << "Result: " << sum << "\n\n" << std::endl;

  sum = 0;
  for (auto &n : randValues) {
    sum.fetch_add(n);
  }
  std::cout << "Time for mySumition " << dur.count() << "seconds" << std::endl;
  std::cout << "Result: " << sum << "\n\n" << std::endl;
}
