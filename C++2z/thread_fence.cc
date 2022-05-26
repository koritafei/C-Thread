/**
 * @file thread_fence.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 栅栏操作
 * @version 0.1
 * @date 2020-07-29
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

class Fence {
public:
    Fence() : _x(false), _y(false), _z(0) {}

    void write_x_then_y() {
        _x.store(true, std::memory_order_relaxed);            // relaxed store
        std::atomic_thread_fence(std::memory_order_release);  // release fence
        _y.store(true, std::memory_order_relaxed);
    }

    void read_y_then_x() {
        while (!_y.load(std::memory_order_relaxed)) {
        }
        std::atomic_thread_fence(std::memory_order_acquire);  // get fence
        if (_x.load(std::memory_order_acquire)) {
            ++_z;
        }
    }

    void print() {
        std::cout << "z = " << _z.load() << std::endl;
    }

private:
    std::atomic<bool> _x, _y;
    std::atomic<int>  _z;
};

int main(int argc, char **argv) {
    std::shared_ptr<Fence> ptr = std::make_shared<Fence>();
    std::thread            t1{&Fence::write_x_then_y, ptr};
    std::thread            t2{&Fence::read_y_then_x, ptr};
    std::thread            t3{&Fence::print, ptr};

    t3.join();
    t2.join();
    t1.join();
}
