/**
 * @file for_each.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 并发for_each
 * @version 0.1
 * @date 2020-08-13
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <algorithm>
#include <functional>
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
  unsigned long const max_threads =
      (length + min_per_thread - 1) / min_per_thread;
  unsigned long const hard_ware_threads = std::thread::hardware_concurrency();
  unsigned long const num_threads =
      std::min(hard_ware_threads != 0 ? hard_ware_threads : 2, max_threads);
  unsigned long const            block_size = length / num_threads;
  std::vector<std::future<void>> futures{num_threads - 1};
  std::vector<std::thread>       threads{num_threads - 1};
  // join_threads                   joiner{threads};
  Iterator block_start = first;
  for (unsigned long i = 0; i < (num_threads - 1); i++) {
    Iterator block_end = block_start;
    std::advance(block_end, block_size);
    std::packaged_task<void(void)> task{[=]() {
      std::for_each(block_start, block_end, f);
    }};
    futures[i]  = task.get_future();
    threads[i]  = std::thread{std::move(task)};
    block_start = block_end;
  }

  std::for_each(block_start, last, f);
  for (unsigned long i = 0; i < (num_threads - 1); i++) {
    futures[i].get();
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
