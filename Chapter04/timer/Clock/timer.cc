#include "timer.h"

using namespace inter_timer;

/**
 * @brief 设置时钟
 *
 * @param hour
 * @param minute
 * @param second
 */
void Timer::set(int hour, int minute, int second) {
  this->_hour   = hour;
  this->_minute = minute;
  this->_second = second;
}

/**
 * @brief 获取当前时间
 *
 * @return int
 */
int Timer::get() {
  return _hour * 10000 + _minute * 100 + _second;
}

/**
 * @brief 时间每秒前进时，时分秒变化
 *
 */
void Timer::next() {
  if (SECONDS > _second) {
    _second++;
  } else if (MINUTES > _minute) {
    _second = 0;
    _minute++;
  } else if (HOURS > _hour) {
    _minute = 0;
    _hour++;
  } else {
    _hour = 0;
  }
}
