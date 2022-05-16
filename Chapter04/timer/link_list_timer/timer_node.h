#ifndef _TIMER_NODE_H_
#define _TIMER_NODE_H_

#include <kenel/linux/list.h>

typedef int timer_id;

/**
 * @brief 到达定时时，调用的回调函数
 *
 * @param id
 * @param user_data
 * @param len
 * @return typedef timer_expiry
 */
typedef int timer_expiry(timer_id id, void *user_data, int len);

struct timer {
  LIST_ENTRY(timer) entries;
  timer_id id;
  int interval;
  int elapse;
  timer_expiry *cb;
  void *usr_data;
  int len;
};

#endif // !_TIMER_NODE_H_