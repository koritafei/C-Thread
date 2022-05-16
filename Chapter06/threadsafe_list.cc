/**
 * @file threadsafe_list.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 线程安全链表
 * @version 0.1
 * @date 2020-08-10
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

template <typename T>
class threadsafe_list {
public:
  threadsafe_list() {
  }
  ~threadsafe_list() {
    remove_if([](node const &) {
      return true;
    });
  }

  threadsafe_list(const threadsafe_list &) = delete;
  threadsafe_list &operator=(const threadsafe_list &) = delete;

  void push_front(T const &value) {
    std::unique_ptr<node>       new_node(new node(value));
    std::lock_guard<std::mutex> lk{head.m};
    new_node->next = std::move(head.next);
    head.next      = std::move(new_node);
  }

  template <typename Function>
  void for_each(Function f) {
    node *                       current = &head;
    std::unique_lock<std::mutex> uk{head.m};
    while (node *const next = current->next.get()) {
      std::unique_lock<std::mutex> next_uk{next->m};
      uk.unlock();
      f(*next->data);
      current = next;
      uk      = std::move(next_uk);
    }
  }

  template <typename Predicate>
  std::shared_ptr<T> find_first_if(Predicate p) {
    node *                       current = &head;
    std::unique_lock<std::mutex> lk{head.m};
    while (node *const next = current->next.get()) {
      std::unique_lock<std::mutex> next_lk{current->m};
      lk.unlock();
      if (p(*next->data)) {
        return next->data;
      }
      current = next;
      lk      = std::move(next_lk);
    }
    return std::shared_ptr<T>();
  }

  template <typename Predicate>
  void remove_if(Predicate p) {
    node *                       current = &head;
    std::unique_lock<std::mutex> lk{head.m};
    while (node *const next = current->next.get()) {
      std::unique_lock<std::mutex> next_lk{next->m};
      if (p(*next->data)) {
        std::unique_ptr<node> old_next = std::move(current->next);
        current->next                  = std::move(next->next);
        next_lk.unlock();
      } else {
        lk.unlock();
        current = next;
        lk      = std::move(next_lk);
      }
    }
  }

private:
  struct node {
    std::mutex            m;
    std::shared_ptr<T>    data;
    std::unique_ptr<node> next;

    node() : next() {
    }
    node(T const &value) : data(std::make_shared<T>(value)) {
    }
  };

  node head;
};

int main(int argc, char **ragv) {
}
