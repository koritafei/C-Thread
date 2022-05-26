/**
 * @file timer.h
 * @author koritafei (koritafei@gmail.com)
 * @brief 时间类
 * @version 0.1
 * @date 2020-07-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _TIMER_H_
#define _TIMER_H_

#include <ctime>
#include <iostream>
#include <string>

namespace inter_timer {

class Timer {
 public:
  void set(int hour, int minute, int second);
  int  get();
  void next();

 private:
  int       _hour;
  int       _minute;
  int       _second;
  const int SECONDS = 59;
  const int MINUTES = 59;
  const int HOURS   = 23;
};  // Timer

}  // namespace inter_timer

#endif  // !_TIMER_H_
