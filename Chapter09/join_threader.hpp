/**
 * @file join_threader.hpp
 * @author koritafei (koritafei@gmail.com)
 * @brief join thread 类
 * @version 0.1
 * @date 2020-08-17
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _JOIN_THREAD_H_
#define _JOIN_THREAD_H_

#include <thread>
#include <vector>

class join_threader {
public:
  explicit join_threader(std::vector<std::thread> &threads_)
      : threads(threads_) {
  }

  ~join_threader() {
    for (unsigned i = 0; i < threads.size(); i++) {
      if (threads[i].joinable()) {
        threads[i].join();
      }
    }
  }

private:
  std::vector<std::thread> &threads;
};

#endif  // !_JOIN_THREAD_H_
