/**
 * @file lock_free_stack.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 无锁数据结构，stack,双计数法
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
class lock_free_stack {
public:
  ~lock_free_stack() {
    while (pop()) {
    }
  }

  void push(T const &data) {
    count_node_ptr new_node;
    new_node.ptr            = new node(data);
    new_node.external_count = 1;
    new_node.ptr->next      = head.load();
    while (!head.compare_exchange_weak(new_node.ptr->next, new_node)) {
    }
  }

  std::shared_ptr<T> pop() {
    count_node_ptr old_head = head.load();
    for (;;) {
      increase_head_count(old_head);
      node *const ptr = old_head.ptr;
      if (!ptr) {
        return std::shared_ptr<T>();
      }
      if (head.compare_exchange_strong(old_head, ptr->next)) {
        std::shared_ptr<T> res;
        res.swap(ptr->data);
        int const count_increase = old_head.external_count - 2;
        if (ptr->internal_count.fetch_add(count_increase) == -count_increase) {
          delete ptr;
        }
        return res;
      } else if (ptr->internal_count.fetch_sub(1) == 1) {
        delete ptr;
      }
    }
  }

private:
  struct node;
  struct count_node_ptr {
    int   external_count;
    node *ptr;
  };

  struct node {
    std::shared_ptr<T> data;
    std::atomic<int>   internal_count;
    count_node_ptr     next;

    node(T const &data_) : data(std::make_shared<T>(data_)), internal_count(0) {
    }
  };

  std::atomic<count_node_ptr> head;

  void increase_head_count(count_node_ptr &old_counter) {
    count_node_ptr new_counter;
    do {
      new_counter = old_counter;
      ++new_counter.external_count;
    } while (!head.compare_exchange_strong(old_counter, new_counter));
    old_counter.external_count = new_counter.external_count;
  }
};

int main(int argc, char **argv) {
  std::shared_ptr<lock_free_stack<int>> ptr =
      std::make_shared<lock_free_stack<int>>();
  std::thread t{&lock_free_stack<int>::push, ptr, std::move(4)};
  t.join();
  std::cout << *(ptr->pop()) << std::endl;
}
