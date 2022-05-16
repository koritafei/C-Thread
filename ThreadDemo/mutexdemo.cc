/**
 * @file mutexdemo.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 互斥元保护共享数据
 * @version 0.1
 * @date 2020-06-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <iostream>
#include <mutex>
#include <thread>
#include <unistd.h>
#include <vector>
#include <functional>
#include <algorithm>

std::mutex _mutex;
int value;

void set_value(int i) {
  while (true) {
    {
      sleep(4);
      std::lock_guard<std::mutex> lk{_mutex};
      value = i;
    }
  }
}

void get_value() {
  while (true) {
    sleep(2);
    std::lock_guard<std::mutex> lk{_mutex};
    std::cout << "thread " << std::this_thread::get_id() << " get value is "
              << value << std::endl;
  }
}

int main(int argc, char **argv) {
  const int thread_nums = 10;
  std::vector<std::thread> product_threads;
  std::vector<std::thread> custom_threads;

  for (int i = 0; i < thread_nums; i++) {
    product_threads.push_back(std::thread{set_value, i});
    custom_threads.push_back(std::thread{get_value});
  }

  std::for_each(custom_threads.begin(), custom_threads.end(),
                std::mem_fn(&std::thread::join));
  std::for_each(product_threads.begin(), product_threads.end(),
                std::mem_fn(&std::thread::join));
  return 0;
}
