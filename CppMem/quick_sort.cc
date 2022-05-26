/**
 * @file quick_sort.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief FP风格编程,快排
 * @version 0.1
 * @date 2020-07-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <future>
#include <iostream>
#include <list>
#include <thread>

// 顺序执行的quick sort
template <typename T>
std::list<T> sequential_quick_sort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }

  std::list<T> result;
  result.splice(result.begin(), input,
                input.begin());  // 将第一个元素放到result

  T const &pivot = *result.begin();
  auto     divide_point =
      std::partition(input.begin(), input.end(), [&](T const &t) {
        return t < pivot;
      });

  std::list<T> lower_part;
  lower_part.splice(lower_part.end(), input, input.begin(), divide_point);
  auto new_lower{sequential_quick_sort(std::move(lower_part))};

  auto new_higher{sequential_quick_sort(std::move(input))};
  result.splice(result.end(), new_higher);
  result.splice(result.begin(), new_lower);

  return result;
}

// 并发的quick sort
template <typename T>
std::list<T> parallel_quick_sort(std::list<T> input) {
  if (input.empty()) {
    return input;
  }

  std::list<T> result;
  result.splice(result.begin(), input, input.begin());  // 第一个元素放入result
  T const &pivot = *result.begin();

  auto divide_point =
      std::partition(input.begin(), input.end(), [&](T const &t) {
        return t < pivot;
      });
  std::list<T> lower_part;
  lower_part.splice(lower_part.end(), input, input.begin(), divide_point);

  std::future<std::list<T>> lower{
      std::async(&parallel_quick_sort<T>, lower_part)};
  auto higher{parallel_quick_sort(std::move(input))};

  result.splice(result.end(), higher);
  result.splice(result.begin(), lower.get());

  return result;
}

template <typename T>
void print(std::list<T> &l) {
  for (auto iter = l.begin(); iter != l.end(); iter++) {
    std::cout << *iter << " ";
  }
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  std::list<int> l{1, 8, 3, 0, 5};
  print(l);
  auto res1 = sequential_quick_sort(l);
  print(res1);
  auto res2 = parallel_quick_sort(l);

  print(res2);
}