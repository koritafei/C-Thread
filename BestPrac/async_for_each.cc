/**
 * @file async_for_each.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief async_实现并发for_each
 * @version 0.1
 * @date 2020-08-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

template <typename Iterator, typename Func>
void parallel_for_each(Iterator first, Iterator last, Func f) {
  unsigned long const length = std::distance(first, last);
  if (!length) {
    return;
  }
  unsigned long const min_per_thread = 25;
  if (length < 2 * min_per_thread) {
    std::for_each(first, last, f);
  } else {
    Iterator const    mid_point = first + length / 2;
    std::future<void> first_half =
        std::async(parallel_for_each<Iterator, Func>, first, mid_point, f);
    parallel_for_each(mid_point, last, f);
    first_half.get();
  }
}

void print(int t) {
  std::cout << "this thread is " << std::this_thread::get_id() << "\tvalue is "
            << t << std::endl;
}

int main(int argc, char **argv) {
  std::vector<int> a = {1, 2, 3, 4, 5, 6, 7};
  parallel_for_each(a.begin(), a.end(), print);
}
