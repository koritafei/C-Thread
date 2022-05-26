/**
 * @file total_order.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 全序
 * @version 0.1
 * @date 2020-07-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <unistd.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <thread>

class TotalOrder {
public:
  TotalOrder(bool x, bool y, int z) : x_(x), y_(y), z_(0) {}

  void write_x() {
    x_.store(true, std::memory_order_seq_cst);
  }

  void write_y() {
    y_.store(true, std::memory_order_seq_cst);
  }

  void read_x_then_y() {
      while (! x_.load(std::memory_order_seq_cst)) {
      }
      if (y_.load(std::memory_order_seq_cst)) {
        --z_;
      }
  }

  void read_y_then_x() {
      while (y_.load(std::memory_order_seq_cst)) {
      }

      if (x_.load(std::memory_order_seq_cst)) {
        ++z_;
      }
  }

  void print_z() {
    std::cout << "z_ = " << z_ << std::endl;
    return ;
  }

private:
  std::atomic<bool> x_, y_;
  std::atomic<int>  z_;
};

int main(int argc, char **argv) {
  std::shared_ptr<TotalOrder> ptr =
      std::make_shared<TotalOrder>(false, false, 0);

  std::thread a{&TotalOrder::write_x, ptr};
  std::thread b{&TotalOrder::write_y, ptr};
  std::thread c{&TotalOrder::read_x_then_y, ptr};
  std::thread d{&TotalOrder::read_y_then_x, ptr};
  std::thread e{&TotalOrder::print_z, ptr};

  a.join();
  b.join();
  c.join();  // c与d交换位置，z_的值不相同
  d.join();
  // c.join();
  e.join();
}
