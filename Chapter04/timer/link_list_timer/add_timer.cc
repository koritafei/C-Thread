/**
 * @file add_timer.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 增加一个Timer
 * @version 0.1
 * @date 2020-06-27
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "timer_list.h"
#include "timer_node.h"
#include <cstdio>
#include <iostream>

/**
 * @brief 创建timer_list
 *
 * @param count timer元素的最大值
 * @return int 0-ok，其他表示错误
 */
int init_timer(int count) {
    int ret = 0;
    if (count < 0 || count > MAX_TIMER_NUM) {
        printf("the timer max number MUST less than %d.\n", MAX_TIMER_NUM);
        return -1;
    }

    memset(&timer_list, 0, sizeof(timer_list));
    LIST_INIT(&timer_list.header);
    timer_list.max_num = count;
    if (SIG_ERR == (timer_list.old_sigfunc = signal(SIGALRM, sig_func))) {
        return -1;
    }

    timer_list.new_sigfunc               = sig_func;
    timer_list.value.it_value.tv_sec     = TIMER_START;
    timer_list.value.it_value.tv_usec    = 0;
    timer_list.value.it_interval.tv_sec  = TIMER_TICK;
    timer_list.value.it_interval.tv_usec = 0;

    ret = setitimer(ITIMER_REAL, &timer_list.value, &timer_list.ovalue);

    return ret;
}

/**
 * @brief 销毁timer_list
 *
 * @return int 0-ok,其他-失败
 */
int destory_timer(void) {
    struct timer *node = nullptr;

    if (SIG_ERR == signal(SIGALRM, time_list.old_sigfunc)) {
        return -1;
    }

    if (0 > (setitimer(ITIMER_REAL, &timer_list.ovalue, &time_list.value))) {
        return -1;
    }

    while (!LIST_EMPTY(&time_list.header)) {  // delete
        node = LIST_FIRST(&timer_list.header);
        LIST_REMOVE(node, entries);
        // free node
        printf("Remove id %d\n", node->id);
        free(node->user_data);
        free(node);
    }
    memset(&time_list, 0, sizeof(time_list));

    return 0;
}

/**
 * @brief 添加一个定时器
 *
 * @param interval
 * @param cb
 * @param user_data
 * @param len
 * @return timer_id
 */
timer_id add_timer(int interval, timer_expiry *cb, void *user_data, int len) {
    struct timer *node = nullptr;
    if (nullptr == cb || 0 >= interval) {
        return INVALID_TIMER_ID;
    }

    if (timer_list.max_num > timer_list.num) {
        timer_list.num++;
    } else {
        return INVALID_TIMER_ID;
    }

    if (nullptr == (node = malloc(struct timer))) {
        return INVALID_TIMER_ID;
    }

    if (nullptr != user_data || 0 != len) {
        node->user_data = malloc(len);
        memcpy(node->user_data, user_data, len);
        node->len = len;
    }

    node->cb       = cb;
    node->internal = interval;
    node->elapse   = 0;
    node->id       = timer_list.num;
    LIST_INSERT_HEAD(&timer_list.header, node, entries);

    return node->id;
}

/**
 * @brief 信号处理函数驱动定时器
 *
 * @param signo
 */
static void sig_func(int signo) {
    struct timer *node = timer_list.header.lh_first;
    for (; node != nullptr; node = node->entries.le_next) {
        node->elapse++;
        if (node->elapse >= node->interval) {
            node->elapse = 0;
            node->cb(node->id, node->user_data, node->len);
        }
    }
}