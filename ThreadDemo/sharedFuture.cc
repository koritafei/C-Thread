#include <future>
#include <iostream>
#include <thread>
#include <utility>

std::mutex countMutex_;

struct Div {
  void operator()(std::promise<int> &&intPromise, int a, int b) {
    intPromise.set_value(a / b);
  }
};

struct Requestor {
  void operator()(std::shared_future<int> shaFut) {
    std::lock_guard<std::mutex> coutGuard{countMutex_};
    std::cout << "ThreadId ( " << std::this_thread::get_id() << " ), ";
    std::cout << "20 / 10 = " << shaFut.get() << std::endl;
  }
};

int main() {
  std::promise<int>       divPromise;
  std::shared_future<int> divResult = divPromise.get_future();

  Div         div;
  std::thread divThread{div, std::move(divPromise), 20, 10};
  Requestor   req;

  std::thread shareThread1{req, divResult};
  std::thread shareThread2{req, divResult};
  std::thread shareThread3{req, divResult};
  std::thread shareThread4{req, divResult};

  divThread.join();
  shareThread1.join();
  shareThread2.join();
  shareThread3.join();
  shareThread4.join();
}
