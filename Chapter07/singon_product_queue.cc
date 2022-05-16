/**
 * @file singon_product_queue.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 线程安全的单生产者队列
 * @version 0.1
 * @date 2020-08-12
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

template <typename T>
class single_lock_queue {
public:
  single_lock_queue() : head(new node), tail(head.load()) {
  }

  single_lock_queue(single_lock_queue &) = delete;
  single_lock_queue &operator=(const single_lock_queue &) = delete;

  ~single_lock_queue() {
    while (node *const old_head = head.load()) {
      head.store(old_head->next);
      delete old_head;
    }
  }

  std::shared_ptr<T> pop() {
    node *old_head = pop_head();
    if (!old_head) {
      return std::shared_ptr<T>();
    }
    std::shared_ptr<T> const res(old_head->data);
    delete old_head;
    return res;
  }

  void push(T new_value) {
    std::shared_ptr<T> new_data = std::make_shared<T>(new_value);
    node *             p        = new node;
    node *const        old_tail = tail.load();
    old_tail->data.swap(new_data);
    tail.store(p);
  }

private:
  struct node {
    std::shared_ptr<T> data;
    node *             next;
    node() : next(nullptr) {
    }
  };

  std::atomic<node *> head;
  std::atomic<node *> tail;
  node *              pop_head() {
    node *const old_head = head.load();
    if (tail.load() == old_head) {
      return nullptr;
    }
    head.store(old_head->next);
    return old_head;
  }
};

int main(int argc, char **argv) {
  std::shared_ptr<single_lock_queue<int>> ptr =
      std::make_shared<single_lock_queue<int>>();
  std::thread t{&single_lock_queue<int>::push, ptr, std::move(4)};
  t.join();
  std::cout << *(ptr->pop()) << std::endl;
}
