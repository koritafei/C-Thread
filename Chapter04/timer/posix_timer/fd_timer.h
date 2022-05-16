/**
 * @file fd_timer.h
 * @author koritafei (koritafei@gmail.com)
 * @brief Linux 提供的基于文件描述符的定时器接口
 * @version 0.1
 * @date 2020-07-03
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef _FD_TIMER_H_
#define _FD_TIMER_H_

#include <sys/timerfd.h>

int timerfd_create(int clock_id, int flags);
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value,
                    struct itimerspec *old_value);
int timerfd_gettime(int fd, struct itimerspec *curr_value);

#endif  // !_FD_TIMER_H_
