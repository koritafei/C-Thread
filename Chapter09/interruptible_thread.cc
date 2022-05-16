/**
 * @file interruptible_thread.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 中断线程实现
 * @version 0.1
 * @date 2020-08-19
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <condition_variable>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

void interrupt_point() {
}

class interrupt_flag {
public:
  interrupt_flag() : thread_cond_(0), thread_cond_any_(0) {
  }

  void set() {
    flag_.store(true, std::memory_order_relaxed);
    std::lock_guard<std::mutex> lk{set_clear_muetx_};
    if (thread_cond_) {
      thread_cond_->notify_all();
    } else if (thread_cond_any_) {
      thread_cond_any_->notify_all();
    }
  }

  template <typename Lockable>
  void wait(std::condition_variable_any &cv, Lockable &lk) {
    struct custom_lock {
      interrupt_flag *self;
      Lockable &      lk;
      custom_lock(interrupt_flag *self_, std::condition_variable_any &cond_,
                  Lockable &lk_)
          : self(self_),
            lk(lk_) {
        self->set_clear_muetx_.lock();
        self->thread_cond_any_ = &cond_;
      }

      void unlock() {
        lk.unlock();
        self->set_clear_muetx_.unlock();
      }

      void lock() {
        std::lock(self->set_clear_muetx_, lk);
      }

      ~custom_lock() {
        self->thread_cond_any_ = 0;
        self->set_clear_muetx_.unlock();
      }
    };

    custom_lock cl{this, cv, lk};
    interrupt_point();
    cv.wait(cl);
    interrupt_point();
  }

private:
  std::atomic<bool>            flag_;
  std::condition_variable *    thread_cond_;
  std::condition_variable_any *thread_cond_any_;
  std::mutex                   set_clear_muetx_;
};