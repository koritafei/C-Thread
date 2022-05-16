/**
 * @file pqtimer.h
 * @author koritafei (koritafei@gmail.com)
 * @brief 最小堆定时器
 * @version 0.1
 * @date 2020-07-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <unordered_map>
#include <vector>

#include "clock.h"
#include "timerqueuebase.h"

class PQTimer : public TimerQueueBase {
public:
  struct TimerNode;

  PQTimer();
  ~PQTimer();

  int  Schedule(uint32_t time_units, TimerCallBack cb) override;
  bool Cancel(int id) override;
  int  Update(int64_t now = 0) override;

  int Size() const override {
    return (int)_heap.size();
  }

private:
  void clear();
  bool siftdown(int x, int n);
  void siftup(int j);
  void Cancel(TimerNode *node);

  std::vector<TimerNode *>             _heap;
  std::unordered_map<int, TimerNode *> _ref;
};
