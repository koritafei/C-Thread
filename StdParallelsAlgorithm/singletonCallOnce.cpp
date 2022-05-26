#include <chrono>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>

constexpr auto tenMill = 10000000;

class MySingLeton {
public:
  static MySingLeton &getInstance() {
    std::call_once(initInstanceFlag, &MySingLeton::initInstance);
    volatile int dummy{};

    return *instance;
  }

private:
  MySingLeton()                    = default;
  ~MySingLeton()                   = default;
  MySingLeton(const MySingLeton &) = delete;
  MySingLeton &operator=(const MySingLeton &) = delete;

  static MySingLeton   *instance;
  static std::once_flag initInstanceFlag;

  static void initInstance() {
    instance = new MySingLeton();
  }
};

MySingLeton   *MySingLeton::instance = nullptr;
std::once_flag MySingLeton::initInstanceFlag;

std::chrono::duration<double> getTime() {
  constexpr auto fourtyMill = 4 * tenMill;
  const auto     begin      = std::chrono::steady_clock::now();

  for (size_t i = 0; i <= fourtyMill; i++) {
    MySingLeton::getInstance();
  }

  const auto end = std::chrono::steady_clock::now() - begin;

  return end;
}

int main() {
  auto       fut1  = std::async(std::launch::async, getTime);
  auto       fut2  = std::async(std::launch::async, getTime);
  auto       fut3  = std::async(std::launch::async, getTime);
  auto       fut4  = std::async(std::launch::async, getTime);
  const auto total = fut1.get() + fut2.get() + fut3.get() + fut4.get();

  std::cout << "total : " << total.count() << std::endl;
}
