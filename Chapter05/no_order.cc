/**
 * @file no_order.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 自由序列
 * @version 0.1
 * @date 2020-07-20
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <iostream>
#include <thread>

std::atomic<int>   x(0), y(0), z(0);
std::atomic<bool>  go(false);
unsigned const int loop_count = 10;

struct read_values {
  int x_, y_, z_;
};

read_values values1[loop_count];
read_values values2[loop_count];
read_values values3[loop_count];
read_values values4[loop_count];
read_values values5[loop_count];

void increment(std::atomic<int> *var_to_inc, read_values *values) {
  while (!go) {
    std::this_thread::yield();  // 自旋，等待信号
  }

  for (unsigned i = 0; i < loop_count; ++i) {
    values[i].x_ = x.load(std::memory_order_relaxed);
    values[i].y_ = y.load(std::memory_order_relaxed);
    values[i].z_ = z.load(std::memory_order_relaxed);
    var_to_inc->store(i + 1, std::memory_order_relaxed);
    std::this_thread::yield();
  }
}

void read_value(read_values *values) {
  while (!go) {
    std::this_thread::yield();
  }

  for (unsigned i = 0; i < loop_count; i++) {
    values[i].x_ = x.load(std::memory_order_relaxed);
    values[i].y_ = y.load(std::memory_order_relaxed);
    values[i].z_ = z.load(std::memory_order_relaxed);

    std::this_thread::yield();
  }
}

void print(read_values *v) {
  for (unsigned i = 0; i < loop_count; ++i) {
    if (i) {
      std::cout << i << ":";
      std::cout << "(" << v[i].x_ << ", " << v[i].y_ << ", " << v[i].z_
                << "), ";
    }
  }
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  std::thread t1{increment, &x, values1};
  std::thread t2{increment, &y, values2};
  std::thread t3{increment, &z, values3};
  std::thread t4{read_value, values4};
  std::thread t5{read_value, values5};

  go = true;

  t5.join();
  t4.join();
  t3.join();
  t2.join();
  t1.join();

  print(values1);
  print(values2);
  print(values3);
  print(values4);
  print(values5);
}
