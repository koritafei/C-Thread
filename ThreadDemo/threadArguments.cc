#include <chrono>
#include <iostream>
#include <thread>

class Sleeper {
public:
  Sleeper(int &i_) : i(i_) {
  }

  void operator()(int k) {
    for (unsigned int j = 0; j <= 5; j++) {
      std::this_thread::sleep_for(std::chrono::microseconds(100));
      i += k;
    }

    std::cout << std::this_thread::get_id() << std::endl;
  }

private:
  int &i;
};

int main() {
  int valSleeper = 1000;

  std::thread t{Sleeper(valSleeper), 5};
  // t.detach();
  t.join();

  std::cout << "valSleeper = " << valSleeper << std::endl;
}
