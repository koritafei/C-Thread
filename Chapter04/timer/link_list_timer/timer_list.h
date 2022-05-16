/**
 * @file timer_list.h
 * @author koritafei (koritafei@gmail.com)
 * @brief 链表管理定时器
 * @version 0.1
 * @date 2020-06-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _TIMER_LIST_H_
#define _TIMER_LIST_H_

#include <kenel/linux/list.h>

struct timer_list {
  LIST_HEAD(listheader, timer) header;

  int num; // timer 元素类
  int max_num;
  void (*old_sigfunc)(int); // 上一个信号句柄
  void (*new_sigfunc)(int); // 新信号句柄

  struct itimerval ovalue; // old timer value
  struct itimerval value;  // new timer value
};

#endif // !_TIMER_LIST_H_
