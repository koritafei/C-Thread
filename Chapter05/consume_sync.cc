/**
 * @file consume_sync.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief consume 实现同步
 * @version 0.1
 * @date 2020-07-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

struct X {
    int         i;
    std::string s;
};

std::atomic<X *> p;
std::atomic<int> a;

void create_x() {
    X *x = new X;
    x->i = 42;
    x->s = "hello";
    a.store(99, std::memory_order_relaxed);
    p.store(x, std::memory_order_release);
}

void use_x() {
    X *x;
    while (!(x = p.load(std::memory_order_consume))) {
        std::this_thread::sleep_for(std::chrono::microseconds(1));
    }

    std::cout << "X i is " << x->i << std::endl;
    std::cout << "X s is " << x->s << std::endl;
    std::cout << "a is " << a.load(std::memory_order_relaxed) << std::endl;
}

int main(int argc, char **argv) {
    std::thread t1{create_x};
    std::thread t2{use_x};
    t2.join();
    t1.join();
}