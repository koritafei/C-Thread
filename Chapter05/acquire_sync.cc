/**
 * @file acquire_sync.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief
 * @version 0.1
 * @date 2020-07-26
 *
 * @copyright Copyright (c) 2020
 *
 */
#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>

std::atomic<int>  data[5];
std::atomic<bool> sync1(false), sync2(false);

void thread_1() {
    data[0].store(41, std::memory_order_relaxed);
    data[1].store(97, std::memory_order_relaxed);
    data[2].store(17, std::memory_order_relaxed);
    data[3].store(-141, std::memory_order_relaxed);
    data[4].store(2003, std::memory_order_relaxed);
    sync1.store(true, std::memory_order_release);
}

void thread_2() {
    while (!sync1.load(std::memory_order_acquire)) {
    }
    sync2.store(true, std::memory_order_release);
}

void thread_3() {
    while (!sync2.load(std::memory_order_acquire)) {
    }

    std::cout << "Data 0 is " << data[0].load(std::memory_order_relaxed)
              << std::endl;
    std::cout << "Data 1 is " << data[1].load(std::memory_order_relaxed)
              << std::endl;
    std::cout << "Data 2 is " << data[2].load(std::memory_order_relaxed)
              << std::endl;
    std::cout << "Data 3 is " << data[3].load(std::memory_order_relaxed)
              << std::endl;
    std::cout << "Data 4 is " << data[4].load(std::memory_order_relaxed)
              << std::endl;
}

int main(int argc, char **argv) {
    std::thread t1{thread_1};
    std::thread t2{thread_2};
    std::thread t3{thread_3};

    t3.join();
    t2.join();
    t1.join();
}
