#include <iostream>
#include <mutex>
#include <new>
#include <string>
#include <thread>
#include <utility>

class scopedLock {
public:
  explicit scopedLock(std::mutex &m) : mut_(m) {
    mut_.lock();
    std::cout << "Lock the mutex " << &mut_ << std::endl;
  }

  ~scopedLock() {
    std::cout << "Release the mutex " << &mut_ << std::endl;
    mut_.unlock();
  }

private:
  std::mutex &mut_;
};

int main() {
  std::mutex mut;
  scopedLock spl{mut};

  std::cout << "\nBefore local scope " << std::endl;
  {
    std::mutex mut1;
    scopedLock spl{mut1};
  }
  std::cout << "\nAfter local scope " << std::endl;

  std::cout << "\nBefore try-catch lock " << std::endl;
  try {
    std::mutex mut3;
    scopedLock spl{mut3};
    throw std::bad_alloc();
  } catch (std::bad_alloc &e) {
    std::cout << e.what() << std::endl;
  }
  std::cout << "\nAfter try-catch lock " << std::endl;
}