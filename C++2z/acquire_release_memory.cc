/**
 * @file acquire_release_memory.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 获取释放操作影响序列中的释放操作
 * @version 0.1
 * @date 2020-07-26
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

class acquire_release {
public:
    acquire_release() : x_(false), y_(false), z_(0) {}

    void write_x_then_y() {
        x_.store(true, std::memory_order_acquire);
        y_.store(true, std::memory_order_release);
    }

    void read_y_then_x() {
        while (!y_.load(std::memory_order_acquire)) {
        }
        if (x_.load(std::memory_order_relaxed)) {
            ++z_;
        }
    }

    int get_value_z() {
        return z_.load();
    }

private:
    std::atomic<bool> x_, y_;
    std::atomic<int>  z_;
};

int main(int argc, char **argv) {
    std::shared_ptr<acquire_release> ptr = std::make_shared<acquire_release>();
    std::thread                      a{&acquire_release::write_x_then_y, ptr};
    std::thread                      b{&acquire_release::read_y_then_x, ptr};
    b.join();
    a.join();

    std::cout << "Z = " << ptr->get_value_z() << std::endl;
}
