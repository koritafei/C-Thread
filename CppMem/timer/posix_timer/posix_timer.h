/**
 * @file posix_timer.h
 * @author koritafei (koritafei@gmail.com)
 * @brief posix_timer 接口
 * @version 0.1
 * @date 2020-07-02
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef _POSIX_TIMER_H_
#define _POSIX_TIMER_H_

#include <signal.h>
#include <time.h>

int timer_create(clockid_t clockid, struct sigevent *evp,
                 time_t *timer_id); // 创建一个定时器
int timer_settime(time_t timerid, int flags, const itimerspec *new_value,
                  itimerspec *old_value); // 启动或者停止一个定时器

int timer_gettime(
    timer_t timerid,
    struct itimerspec
        *curr_value); //  返回到下一次到期的剩余时间值和定时器定义的时间间隔
int timer_getoverrun(timer_t timerid); // 返回上次定时器到期时超限值
int timer_delete(timer_t timerid);     // 停止并删除一个定时器

// POSIX timer接口中的信号和时间定义
union sigval {
  int sival_int;
  void *sival_ptr;
};

struct sigevent{
  int sigev_notify; // 通知方式
  int sigev_signo; // 时间到的信号
  union sigval sigev_value; 
  void (* sigev_notify_function)(union sigval);
  void *sigev_notify_attributes;
  pid_t sigev_notify_thread_id;
};

#endif //_POSIX_TIMER_H_
