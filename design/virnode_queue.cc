/**
 * @file virnode_queue.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 带虚拟头结点的线程安全链表
 * @version 0.1
 * @date 2020-07-30
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

template <typename T>
class queue {
public:
    queue() : _head(new node()), _tail(_head.get()) {}

    queue(const queue &) = delete;
    queue &operator=(const queue &) = delete;

    std::shared_ptr<T> try_pop() {
        if (_head.get() == _tail) {
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> const res(_head->_data);
        std::unique_ptr<node>    old_head = std::move(_head);
        _head                             = std::move(old_head->_next);

        return res;
    }

    void push(T new_value) {
        std::shared_ptr<T> new_data = std::make_shared<T>(std::move(new_value));
        std::unique_ptr<node> p{new node};
        _tail->_data         = new_data;
        node *const new_tail = p.get();
        _tail->_next         = std::move(p);
        _tail                = new_tail;
    }

private:
    struct node {
        std::shared_ptr<T>    _data;
        std::unique_ptr<node> _next;
    };
    std::unique_ptr<node> _head;
    node *                _tail;
};

int main(int argc, char **argv) {
    std::shared_ptr<queue<int>> ptr = std::make_shared<queue<int>>();
    ptr->push(5);
    std::cout << *(ptr->try_pop()) << std::endl;
}
