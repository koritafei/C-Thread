/**
 * @file sleep_for.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief sleep_for与sleep_until函数
 * @version 0.1
 * @date 2020-07-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <chrono>
#include <cstdio>
#include <ctime>
#include <functional>
#include <future>
#include <iomanip>
#include <iostream>
#include <memory>
#include <thread>
#include <tuple>
#include <utility>

/**
 * @brief clock function
 *
 */

class period_tasks {
 public:
  template <typename Func, typename... Args>
  void period_task(uint64_t &time, Func &&f, Args &&... args) {
    auto func = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(time));  // 等待时间
      // run task
      std::cout << "period task ";
      func();
    }
  }

  template <typename Func, typename... Args>
  void period_until_task(struct std::tm *now, const int &hour, const int &min,
                         const int &sec, Func &&f, Args &&... args) {
    auto func = std::bind(std::forward<Func>(f), std::forward<Args>(args)...);
    while (true) {
      std::this_thread::sleep_until(
          std::chrono::system_clock::from_time_t(std::mktime(now)));
      std::cout << "Period until task ";
      func();
      time_init(now, hour, min, sec);
    }
  }

 private:
  /**
   * @brief use to change time point to next date
   *
   * @param now
   * @param hour
   * @param min
   * @param sec
   */
  void time_init(struct std::tm *now, const int &hour, const int &min,
                 const int &sec) {
    auto time    = std::chrono::system_clock::now();
    time         = time + std::chrono::seconds(86400);  // 加一天
    auto t       = std::chrono::system_clock::to_time_t(time);
    now          = std::localtime(&t);
    now->tm_hour = hour;
    now->tm_min  = min;
    now->tm_sec  = sec;
  }
};

void print() {
  auto now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct std::tm *time = std::localtime(&now);
  std::cout << "Current time is " << std::put_time(time, "%x")
            << std::put_time(time, "%X") << std::endl;
}

int main(int argc, char **argv) {
  std::shared_ptr<period_tasks> period_tasks_ptr =
      std::make_shared<period_tasks>();
  // init time point and duration
  uint64_t duration = 1;
  auto     now =
      std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
  struct std::tm *time = std::localtime(&now);

  // change to first run time point
  const int hour = 19;
  const int min  = 23;
  const int sec  = 0;
  time->tm_hour  = hour;
  time->tm_min   = min;
  time->tm_sec   = sec;
  std::thread d([&]() {
    period_tasks_ptr->period_task(duration, print);
  });

  std::thread util([&]() {
    period_tasks_ptr->period_until_task(time, hour, min, sec, print);
  });
  util.join();
  d.join();
}
