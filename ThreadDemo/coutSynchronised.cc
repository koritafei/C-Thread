#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

std::mutex mutex_;

class Worker {
public:
  Worker(std::string name) : name_(name) {
  }

  void operator()() {
    for (int i = 1; i <= 3; i++) {
      std::this_thread::sleep_for(std::chrono::microseconds(200));
      mutex_.lock();
      std::cout << name_ << " : Worker " << i << " Done! " << std::endl;
      mutex_.unlock();
    }
  }

private:
  std::string name_;
};

int main() {
  std::cout << std::endl;
  std::cout << "Boss : Let's Start Worker!";

  std::thread herb   = std::thread{Worker("Herb")};
  std::thread andrei = std::thread{Worker(" Andei")};
  std::thread scott  = std::thread{Worker("  Scott")};
  std::thread bjarne = std::thread{Worker(" Bjarne")};
  std::thread bart   = std::thread{Worker(" Bart")};
  std::thread jenne  = std::thread{Worker("  Jenne")};

  herb.join();
  andrei.join();
  scott.join();
  bjarne.join();
  bart.join();
  jenne.join();

  std::cout << "Go Home!" << std::endl;
}
