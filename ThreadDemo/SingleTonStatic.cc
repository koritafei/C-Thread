#include <iostream>
#include <thread>

class SingleTon {
public:
  static SingleTon &getInstance() {
    static SingleTon instance;

    return instance;
  }

private:
  SingleTon()  = default;
  ~SingleTon() = default;

  SingleTon(const SingleTon &) = delete;
  SingleTon &operator=(const SingleTon &) = delete;
};

void threadRun() {
  std::cout << "threadid : " << std::this_thread::get_id()
            << " ,singleton address : " << &SingleTon::getInstance()
            << std::endl;
}

int main() {
  std::cout << "threadid : " << std::this_thread::get_id()
            << " ,singleton address : " << &SingleTon::getInstance()
            << std::endl;

  std::thread t1{threadRun};
  std::thread t2{threadRun};

  t1.join();
  t2.join();

  std::cout << "threadid : " << std::this_thread::get_id()
            << " ,singleton address : " << &SingleTon::getInstance()
            << std::endl;
}
