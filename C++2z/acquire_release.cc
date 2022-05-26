/**
 * @file acquire_release.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 获取释放队列
 * @version 0.1
 * @date 2020-07-21
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <iostream>
#include <thread>
#include <cassert>

std::atomic<bool> x, y;
std::atomic<int>  z;

void write_x() {
  x.store(true, std::memory_order_release);
}

void write_y() {
  y.store(true, std::memory_order_release);
}

void read_x_then_y() {
  while (!x.load(std::memory_order_acquire));

  if (y.load(std::memory_order_acquire)) {
    ++z;
  }
}

void read_y_then_x() {
  while (!y.load(std::memory_order_acquire)); 
  
  if (x.load(std::memory_order_acquire)) {
    --z;
  }
}

int main(int argc, char **argv) {
  x = false;
  y = false;
  z = 0;

  std::thread t1{write_x};
  std::thread t2{write_y};
  std::thread t3{read_y_then_x};
  std::thread t4{read_x_then_y};

  t4.join();
  t3.join();
  t2.join();
  t1.join();

  std::cout << "z = " << z.load() << std::endl;
}
