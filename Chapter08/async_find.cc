/**
 * @file async_find.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief async 实现并行find
 * @version 0.1
 * @date 2020-08-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

template <typename Iterator, typename MatchType>
Iterator parallel_find_imp(Iterator first, Iterator last, MatchType match,
                           std ::atomic<bool> &done) {
  try {
    unsigned long const length          = std::distance(first, last);
    unsigned long const min_per_threads = 25;
    if (length < 2 * min_per_threads) {
      for (; first != last && !done.load(); ++first) {
        if (*first == match) {
          done = true;
          return first;
        }
      }
      return last;
    } else {
      Iterator const        mid_point = first + length / 2;
      std::future<Iterator> async_result =
          std::async(&parallel_find_imp<Iterator, MatchType>, mid_point, last,
                     match, std::ref(done));
      Iterator const direct_result =
          parallel_find_imp(first, mid_point, match, done);
      return (direct_result == mid_point ? async_result.get() : direct_result);
    }
  } catch (...) {
    done = true;
    throw;
  }
}

template <typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType match) {
  std::atomic<bool> done{false};
  return parallel_find_imp(first, last, match, done);
}

int main(int argc, char **argv) {
  std::vector<int> a{1, 2, 3, 4, 5, 6};
  int              key = 10;
  auto             res = parallel_find(a.begin(), a.end(), key);
  if (res == a.end()) {
    std::cout << "Not Found" << std::endl;
  } else {
    std::cout << "Found!" << *res << std::endl;
  }
}
