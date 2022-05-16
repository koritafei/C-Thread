/**
 * @file RAII.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief RAII等待线程结束
 * @version 0.1
 * @date 2020-06-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <iostream>
#include <thread>

class thread_guard {
public:
  explicit thread_guard(std::thread &t) : _t(t) {}

  ~thread_guard() {
    if (_t.joinable()) {
      _t.join();
    }
  }

  thread_guard(thread_guard const &) = delete;
  thread_guard &operator=(thread_guard const &) = delete;

private:
  std::thread &_t;
};

struct func {
  void operator()(){
    std::cout << "struct thread id is " << std::this_thread::get_id() << std::endl;
    std::cout << "value = " << _i << std::endl;
  }

  func (int &i):_i(i){}

  int &_i;
};

void f(){
  int some_local_state = 10;
  func my_func {std::ref(some_local_state)};
  std::thread t{std::move(my_func)};
  thread_guard g{t};
  std::cout << "this thread id is " << std::this_thread::get_id() << std::endl;
}


int main(int argc,char **argv){
  f();
}
