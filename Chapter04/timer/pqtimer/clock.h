/**
 * @file clock.h
 * @author koritafei (koritafei@gmail.com)
 * @brief Clock时钟定义
 * @version 0.1
 * @date 2020-07-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <cstdint>
#include <string>

enum {
  TIME_UINT = int64_t(10),
};

class Clock {
public:
  static int64_t     CurrentTimeMillis();
  static int64_t     CurrentTimeUnits();
  static std::string CurrentTimeString(int64_t timepoint);
  static uint64_t    GetNowTickCount();
};

#endif
