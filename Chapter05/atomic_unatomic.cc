/**
 * @file atomic_unatomic.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 原子操作对非原子操作进行排序
 * @version 0.1
 * @date 2020-07-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

class Atomic_Untomic {
public:
    void write_x_then_y() {
        _x = true;
        std::atomic_thread_fence(std::memory_order_relaxed);
        _y.store(true, std::memory_order_release);
    }

    void read_y_then_x() {
        while (!_y.load(std::memory_order_relaxed)) {
        }
        std::atomic_thread_fence(std::memory_order_acquire);
        if (_x) {
            ++_z;
        }
    }

    void print() {
        std::cout << "z = " << _z.load() << std::endl;
    }

private:
    bool              _x;
    std::atomic<bool> _y;
    std::atomic<int>  _z;
};

int main(int argc, char **argv) {
    std::shared_ptr<Atomic_Untomic> ptr = std::make_shared<Atomic_Untomic>();
    std::thread                     t1{&Atomic_Untomic::write_x_then_y, ptr};
    std::thread                     t2{&Atomic_Untomic::read_y_then_x, ptr};
    std::thread                     t3{&Atomic_Untomic::print, ptr};

    t3.join();
    t2.join();
    t1.join();
}
