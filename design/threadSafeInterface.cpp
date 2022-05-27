#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>

class Critical {
public:
  void interface1() {
    std::lock_guard<std::mutex> lockguard{_mut};
    implementation1();
  }

  void interface2() {
    std::lock_guard<std::mutex> lockguard{_mut};
    implementation3();
    implementation2();
    implementation1();
  }

private:
  void implementation1() const {
    std::cout << "implementation1: " << std::this_thread::get_id() << std::endl;
  }

  void implementation2() const {
    std::cout << "implementation2: " << std::this_thread::get_id() << std::endl;
  }

  void implementation3() const {
    std::cout << "implementation3: " << std::this_thread::get_id() << std::endl;
  }

  mutable std::mutex _mut;
};

int main() {
  std::thread t1{[] {
    Critical cir;
    cir.interface1();
  }};

  std::thread t2{[] {
    Critical cir;
    cir.interface2();
  }};

  Critical cir;
  cir.interface1();
  cir.interface2();
  t1.join();
  t2.join();
}
