/**
 * @file async_future.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 返回后台任务结果
 * @version 0.1
 * @date 2020-06-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <future>
#include <iostream>
#include <unistd.h>
#include <functional>
#include <algorithm>

int find_answer() {
  std::cout << "thread id is " << std::this_thread::get_id() << std::endl;
  sleep(10);
  return 100;
}

void main_task() {
  std::cout << "thread id is " << std::this_thread::get_id() << std::endl;
  sleep(5);
  std::cout << "num is " << 50 << std::endl;
}

int main(int argc, char **argv) {
  std::future<int> the_answer = std::async(find_answer);
  main_task();
  std::cout << "num is " << the_answer.get() << std::endl;

  return 0;
}
