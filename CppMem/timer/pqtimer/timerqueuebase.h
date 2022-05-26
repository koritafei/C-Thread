/**
 * @file timerqueuebase.h
 * @author koritafei (koritafei@gmail.com)
 * @brief 定时器队列
 * @version 0.1
 * @date 2020-07-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _TIMER_QUEUE_BASE_H_
#define _TIMER_QUEUE_BASE_H_

#include <cstdint>
#include <functional>
#include <unordered_map>

// time_out call back
using TimerCallBack = std::function<void()>;

// 定时器队列
class TimerQueueBase {
public:
  TimerQueueBase();
  virtual ~TimerQueueBase();
  TimerQueueBase(const TimerQueueBase &) = delete;
  TimerQueueBase &operator=(const TimerQueueBase &) = delete;

  virtual int  Schedule(uint32_t time_uints, TimerCallBack cb = 0) = 0;
  virtual bool Cancel(int id)                                      = 0;
  virtual int  Update(int64_t now = 0)                             = 0;

  virtual int Size() const = 0;

protected:
  int nextCounter();

  int _counter = 0;  // next timer id
};

#endif  // _TIMER_QUEUE_BASE_H_
