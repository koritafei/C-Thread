/**
 * @file wait_for.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 超时等待
 * @version 0.1
 * @date 2020-07-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <unistd.h>

#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <thread>

class thread_wait {
 public:
  bool wait_loop() {
    auto const timeout =
        std::chrono::steady_clock::now() + std::chrono::milliseconds(500);
    std::unique_lock<std::mutex> lock{_mutex};
    while (!_done) {
      if (_cv.wait_until(lock, timeout) == std::cv_status::timeout) {
        std::cout << "Thread wait timeout" << std::endl;
        break;
      }
    }

    return _done;
  }

  void set_done(bool done) {
    std::unique_lock<std::mutex> lock{_mutex};
    _done = done;
    _cv.notify_one();
  }

 private:
  std::condition_variable _cv;
  bool                    _done;
  mutable std::mutex      _mutex;
};

int main(int argc, char **argv) {
  std::shared_ptr<thread_wait> threadWaitPtr = std::make_shared<thread_wait>();

  std::thread t1{&thread_wait::wait_loop, threadWaitPtr};
  t1.join();
}
