
#include "timerqueuebase.h"

TimerQueueBase::TimerQueueBase() {}

TimerQueueBase::~TimerQueueBase() {}

int TimerQueueBase::nextCounter() {
  return ++_counter;
}
