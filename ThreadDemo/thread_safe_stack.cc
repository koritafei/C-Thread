/**
 * @file thread_safe_stack.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 线程安全的栈
 * @version 0.1
 * @date 2020-06-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <stack>
#include <thread>
#include <vector>

struct empty_stack : std::exception {
  const char *what() const throw() {
    return "empty stack";
  }
};

template <typename T>
class thread_safe_stack {
public:
  using shared_ptr = std::shared_ptr<T>;

  explicit thread_safe_stack() : _stack(std::stack<T>()) {}

  thread_safe_stack(const thread_safe_stack &other) {
    std::lock_guard<std::mutex> lk{other._mutex};
    _stack = other._stack;
  }

  thread_safe_stack &operator=(const thread_safe_stack &) = delete;

  void push(T &value) {
    std::lock_guard<std::mutex> lk{_mutex};
    _stack.push(value);
  }

  void pop(T &value) {
    std::lock_guard<std::mutex> lk{_mutex};
      if (is_empty()) {
        throw empty_stack();
      }
    value = _stack.top();
    _stack.pop();
  }

  void pop_void() {
    std::lock_guard<std::mutex> lk{_mutex};
      if (is_empty()) {
        throw empty_stack();
      }

    std::cout << "This Thread is " << std::this_thread::get_id()
              << " get value is " << _stack.top() << std::endl;
    _stack.pop();
  }

  shared_ptr pop() {
    std::lock_guard<std::mutex> lk{_mutex};
      if (is_empty()) {
        return empty_stack();
      }
    shared_ptr const res = std::make_shared<T>(_stack.top());
    _stack.pop();
    return res;
  }

  bool is_empty() {
    std::lock_guard<std::mutex> lk{_mutex};
    return _stack.empty();
  }

private:
  std::stack<T>      _stack;
  mutable std::mutex _mutex;
};

int main(int argc, char **argv) {
  std::shared_ptr<thread_safe_stack<int>> stack =
      std::make_shared<thread_safe_stack<int>>();

  const int                thread_nums = 10;
  std::vector<std::thread> product_threads;
  std::vector<std::thread> custom_threads;

    for (int i = 0; i < thread_nums; i++) {
      product_threads.push_back(
          std::thread{&thread_safe_stack<int>::push, stack, std::ref(i)});
      custom_threads.push_back(
          std::thread{&thread_safe_stack<int>::pop_void, stack});
    }

  std::for_each(product_threads.begin(),
                product_threads.end(),
                std::mem_fn(&std::thread::join));
  std::for_each(custom_threads.begin(),
                custom_threads.end(),
                std::mem_fn(&std::thread::join));

  return 0;
}
