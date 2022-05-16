#ifndef _THREDA_SAFE_QUEUE_H_
#define _THREAD_SAFE_QUEUE_H_

#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

template <typename T>
class threadsafe_queue {
public:
  threadsafe_queue() : head(new node), tail(head.get()) {
  }

  threadsafe_queue(const threadsafe_queue &) = delete;
  threadsafe_queue &operator=(const threadsafe_queue &) = delete;

  std::shared_ptr<T> try_pop();
  bool               try_pop(T &value);
  std::shared_ptr<T> wait_and_pop();
  void               wait_and_pop(T &value);
  void               push(T value);
  bool               empty();

private:
  struct node {
    std::shared_ptr<T>    data;
    std::unique_ptr<node> next;
  };

  std::mutex              head_mutex;
  std::unique_ptr<node>   head;
  std::mutex              tail_mutex;
  node *                  tail;
  std::condition_variable data_con;

  node *get_tail() {
    std::lock_guard<std::mutex> lk{tail_mutex};
    return tail;
  }

  std::unique_ptr<node> pop_head() {
    std::unique_ptr<node> old_head = std::move(head);
    head                           = std::move(old_head->next);
    return old_head;
  }

  std::unique_lock<std::mutex> wait_for_data() {
    std::unique_lock<std::mutex> head_lock{head_mutex};
    data_con.wait(head_lock, [&] {
      return head.get() != get_tail();
    });
    return std::move(head_lock);
  }

  std::unique_ptr<node> wait_pop_head() {
    std::unique_lock<std::mutex> head_lock(wait_for_data());
    return pop_head();
  }

  std::unique_ptr<node> wait_pop_head(T &value) {
    std::unique_lock<std::mutex> head_lock{wait_for_data()};
    value = std::move(*head->data);
    return pop_head();
  }

  std::unique_ptr<node> try_pop_head() {
    std::lock_guard<std::mutex> lk{head_mutex};
    if (head.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    return pop_head();
  }

  std::unique_ptr<node> try_pop_head(T &value) {
    std::lock_guard<std::mutex> lk{head_mutex};
    if (head.get() == get_tail()) {
      return std::unique_ptr<node>();
    }
    value = std::move(*head->data);
    return pop_head();
  }
};

template <typename T>
void threadsafe_queue<T>::push(T value) {
  std::shared_ptr<T>    new_data = std::make_shared<T>(std::move(value));
  std::unique_ptr<node> p(new node);
  {
    std::lock_guard<std::mutex> tail_lock{tail_mutex};
    tail->data           = new_data;
    node *const new_tail = p.get();
    tail->next           = std::move(p);
    tail                 = new_tail;
  }
  data_con.notify_one();
}

template <typename T>
std::shared_ptr<T> threadsafe_queue<T>::wait_and_pop() {
  std::unique_ptr<node> const old_head = wait_pop_head();
  return old_head->data;
}

template <typename T>
void threadsafe_queue<T>::wait_and_pop(T &value) {
  std::unique_ptr<node> const old_head = wait_pop_head(value);
}

template <typename T>
std::shared_ptr<T> threadsafe_queue<T>::try_pop() {
  std::unique_ptr<node> old_head = try_pop_head();
  return old_head ? old_head->data : std::shared_ptr<T>();
}

template <typename T>
bool threadsafe_queue<T>::try_pop(T &value) {
  std::unique_ptr<node> old_head = try_pop_head(value);
  return old_head == nullptr;
}

template <typename T>
bool threadsafe_queue<T>::empty() {
  std::lock_guard<std::mutex> lk{head_mutex};
  return head.get() == get_tail();
}

#endif  // !_THREDA_SAFE_QUEUE_H_