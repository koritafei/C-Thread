/**
 * @file thread_safe_queue.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 线程安全队列
 * @version 0.1
 * @date 2020-06-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <condition_variable>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <functional>
#include <algorithm>

template <typename T> class thread_safe_queue {
public:
  thread_safe_queue() {}

  thread_safe_queue(thread_safe_queue const &other) {
    std::lock_guard<std::mutex> lk{_mutex};
    _data = other._data;
  }

  void push(T &value) {
    std::lock_guard<std::mutex> lk{_mutex};
    _data.push(value);
    _conn_var.notify_one();
  }

  void wait_and_pop(T &value) {
    std::unique_lock<std::mutex> uk{_mutex};
    _conn_var.wait(uk, [this] { return !_data.empty(); });
    value = _data.front();
    _data.pop();
  }

  std::shared_ptr<T> pop() {
    std::unique_lock<std::mutex> uk{_mutex};
    _conn_var.wait(uk, [this] { return !_data.empty(); });
    std::shared_ptr<T> res = std::make_shared<T>(_data.front());
    _data.pop();
    return res;
  }

  bool try_pop(T &value) {
    std::lock_guard<std::mutex> lk{_mutex};
    if (_data.empty()) {
      return false;
    }
    value = _data.front();
    _data.pop();

    return true;
  }

  std::shared_ptr<T> try_pop() {
    std::lock_guard<std::mutex> lk{_mutex};
    if (_data.empty()) {
      return nullptr;
    }
    std::shared_ptr<T> res = std::make_shared<T>(_data.front());
    _data.pop();

    return res;
  }

  bool empty() const {
    std::lock_guard<std::mutex> lk{_mutex};
    return _data.empty();
  }

private:
  mutable std::mutex _mutex;
  std::queue<T> _data;
  std::condition_variable _conn_var;
};

int main(int argc, char **argv) {
  std::shared_ptr<thread_safe_queue<int>> safe_queue =
      std::make_shared<thread_safe_queue<int>>();

  int thread_nums = 20;
  int num = 0;
  int t = 30;

  std::cout << "num = " << num << std::endl;

  /*std::vector<std::thread> product_threads;
  std::vector<std::thread> custom_threads;

  for (int i = 0; i < thread_nums; i++) {
  }*/
  std::thread put_thread{&thread_safe_queue<int>::push, safe_queue,
                         std::ref(thread_nums)};
  std::thread get_thread{&thread_safe_queue<int>::wait_and_pop, safe_queue,
                         std::ref(num)};
  safe_queue->push(t);
  put_thread.join();
  get_thread.join();
  std::cout << "num = " << num << std::endl;
  safe_queue->wait_and_pop(num);

  std::cout << "num = " << num << std::endl;

  return 0;
}
