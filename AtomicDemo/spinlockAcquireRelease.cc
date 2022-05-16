#include <atomic>
#include <thread>

class SpinLock {
public:
  SpinLock() : _flag(ATOMIC_FLAG_INIT) {
  }

  void lock() {
    while (_flag.test_and_set(std::memory_order_acquire))
      ;
  }

  void unlock() {
    _flag.clear(std::memory_order_release);
  }

private:
  std::atomic_flag _flag;
};

SpinLock slock;

void WorkOnResrce() {
  slock.lock();
  slock.unlock();
}

int main(int argc, char **argv) {
  std::thread p1{WorkOnResrce};
  std::thread p2{WorkOnResrce};

  p1.join();
  p2.join();
}
