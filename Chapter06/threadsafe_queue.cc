/**
 * @file threadsafe_queue.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 细粒度锁
 * @version 0.1
 * @date 2020-08-02
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

template <typename T> class threadsafe_queue {
public:
  threadsafe_queue()
      : head(new node), tail(head.get()) {
  } // 初始化列表顺序不定，初始化顺序与变量定义顺序有关

  threadsafe_queue(const threadsafe_queue &) = delete;
  threadsafe_queue &operator=(const threadsafe_queue &) = delete;

  std::shared_ptr<T> try_pop() {
    std::unique_ptr<node> old_head = pop_head();
    return old_head ? old_head->data : std::shared_ptr<T>();
  }

  void push(T new_value) {
    std::shared_ptr<T> new_data = std::make_shared<T>(std::move(new_value));
    std::unique_ptr<node> p{new node};
    node *const new_tail = p.get();
    std::lock_guard<std::mutex> tail_lock(tail_mutex);
    tail->data = new_data;
    tail->next = std::move(p);
    tail = new_tail;
  }

private:
  struct node {
    std::shared_ptr<T> data;
    std::unique_ptr<node> next;
  };

  std::mutex head_mutex;
  std::unique_ptr<node> head;
  std::mutex tail_mutex;
  node *tail;

  node *get_tail() {
    std::lock_guard<std::mutex> lk{tail_mutex};
    return tail;
  }

  std::unique_ptr<node> pop_head() {
    std::lock_guard<std::mutex> lk{head_mutex};
    if (get_tail() == head.get()) {
      return nullptr;
    }
    std::unique_ptr<node> old_head = std::move(head);
    head = std::move(old_head->next);
    return old_head;
  }
};

int main(int argc, char **argv) {
  std::shared_ptr<threadsafe_queue<int>> ptr =
      std::make_shared<threadsafe_queue<int>>();
  std::thread t1{&threadsafe_queue<int>::push, ptr, 5};
  t1.join();
  std::cout << *(ptr->try_pop()) << std::endl;
}
