/**
 * @file atomic_product.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 原子操作实现生产者消费者
 * @version 0.1
 * @date 2020-07-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <unistd.h>

#include <algorithm>
#include <atomic>
#include <functional>
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

class product {
public:
  product() : data_reary_(false) {}

  void reader_thread() {
      while (! data_reary_.load()) {
        sleep(1);
      }
    std::cout << data_.back() << std::endl;
    data_.pop_back();
    data_reary_.store(false);
  }

  void write_thread(int &val) {
    data_.push_back(std::move(val));
    data_reary_ = true;
  }

private:
  std::vector<int>  data_;
  std::atomic<bool> data_reary_;
};

int main(int argc, char **argv) {
  std::shared_ptr<product> product_ptr = std::make_shared<product>();

  std::vector<std::thread> product;
  std::vector<std::thread> custom;
  const int                thread_num = 1;

    for (int i = 0; i < thread_num; i++) {
      product.push_back(
          std::thread{&product::write_thread, product_ptr, std::ref(i)});
      custom.push_back(std::thread{&product::reader_thread, product_ptr});
    }

  std::for_each(custom.begin(), custom.end(), std::mem_fn(&std::thread::join));

  std::for_each(product.begin(),
                product.end(),
                std::mem_fn(&std::thread::join));
}
