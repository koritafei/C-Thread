#include <chrono>
#include <iostream>

constexpr auto tenMill = 10000000;

class MySingLeton {
public:
  static MySingLeton &getInstance() {
    static MySingLeton instance;

    volatile int dummy{};

    return instance;
  }

private:
  MySingLeton()                    = default;
  ~MySingLeton()                   = default;
  MySingLeton(const MySingLeton &) = delete;
  MySingLeton &operator=(const MySingLeton &) = delete;
};

int main() {
  constexpr auto fourtyMill = 4 * tenMill;
  const auto     begin      = std::chrono::steady_clock::now();

  for (size_t i = 0; i <= fourtyMill; i++) {
    MySingLeton::getInstance();
  }

  const auto end = std::chrono::steady_clock::now() - begin;

  std::cout << std::chrono::duration<double>(end).count() << std::endl;
}