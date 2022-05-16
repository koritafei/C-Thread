/**
 * @file simple_thread_pool.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 简单线程池实现
 * @version 0.1
 * @date 2020-08-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <functional>
#include <vector>

#include "join_threader.hpp"
#include "threadsafe_queue.hpp"

class thread_pool {
public:
  thread_pool() : done(false), joiner(threads) {
    unsigned const thread_count = std::thread::hardware_concurrency();
    try {
      for (unsigned i = 0; i < thread_count; i++) {
        threads.push_back(std::thread{&thread_pool::work_thread, this});
      }
    } catch (...) {
      done = true;
      throw;
    }
  }

  ~thread_pool() {
    done = true;
  }

  template <typename FunctionType>
  void submit(FunctionType f) {
    work_queue.push(std::function<void()>(f));
  }

private:
  std::atomic_bool                        done;
  threadsafe_queue<std::function<void()>> work_queue;
  std::vector<std::thread>                threads;
  join_threader                           joiner;

  void work_thread() {
    while (!done) {
      std::function<void()> task;
      if (work_queue.try_pop(task)) {
        task();
      } else {
        std::this_thread::yield();
      }
    }
  }
};

void print() {
  std::cout << "This thread is " << std::this_thread::get_id() << std::endl;
}

int main(int argc, char **argv) {
  std::shared_ptr<thread_pool> tp = std::make_shared<thread_pool>();
  for (unsigned i = 0; i < 20; i++) {
    tp->submit(print);
  }
}
