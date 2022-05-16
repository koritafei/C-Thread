#include <atomic>
#include <memory>
#include <thread>

int main(int argc, char **argv) {
  std::shared_ptr<int> ptr = std::make_shared<int>(2011);

  for (auto i = 0; i < 10; i++) {
    std::thread([ptr] { // ptr 传入复制值
      std::shared_ptr<int> localPtr(
          ptr); // 采用复制构造，每个线程都有自己的一份数据
      localPtr = std::make_shared<int>(2014); // 修改线程内数据的值，不影响ptr
    })
        .detach();
  }

  std::shared_ptr<int> ptr1 = std::make_shared<int>(2011);
  for (auto i = 0; i < 10; i++) {
    std::thread([&ptr1] {
      auto localPtr = std::make_shared<int>(2014);
      std::atomic_store(&ptr1, localPtr); // 避免产生竞争
    }).detach();
  }
}