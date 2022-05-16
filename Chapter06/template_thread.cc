/**
 * @file template_thread.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 模板函数和模板类在线程中的应用
 * @version 0.1
 * @date 2020-08-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <iostream>
#include <memory>
#include <thread>

template <typename T> void print(T const &t) {
  std::cout << "value is " << t << std::endl;
}

template <typename T> class template_class {
public:
  void print(T const &t) {
    std::cout << "class value is " << t << std::endl;
  }
};

int main(int argc, char **argv) {
  int         a = 5;
  std::thread t{print<int>, a};
  t.join();
  std::shared_ptr<template_class<int>> ptr =
      std::make_shared<template_class<int>>();
  std::thread t2{&template_class<int>::print, ptr, 5};
  t2.join();
}