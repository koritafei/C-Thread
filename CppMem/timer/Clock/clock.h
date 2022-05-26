/**
 * @file clock.h
 * @author koritafei (koritafei@gmail.com)
 * @brief 闹钟的实现
 * @version 0.1
 * @date 2020-07-04
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include "timer.h"

namespace inter_clock {

class Clock {
 public:
  void adjust_now(int h, int m, int s);
  void adjust_ring(int h, int m, int s);
  void tick();
  void showtime();
  void run();

 private:
  inter_timer::Timer _now;
  inter_timer::Timer _ring_timer;
};  // Clock

}  // namespace inter_clock

#endif  // !_CLOCK_H_
