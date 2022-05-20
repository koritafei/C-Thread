#include <future>
#include <iostream>
#include <thread>
#include <utility>

std::mutex coutMutex_;

struct Div {
  void operator()(std::promise<int> &&intPromise, int a, int b) {
    intPromise.set_value(a / b);
  }
};

struct Requestor {
  void operator()(std::shared_future<int> shatFut) {
    std::lock_guard<std::mutex> lock{coutMutex_};
    std::cout << "ThreadId ( " << std::this_thread::get_id() << " ) ";
    std::cout << " 20 / 10 = " << shatFut.get() << std::endl;
  }
};

int main() {
  std::cout << std::boolalpha << std::endl;
  std::promise<int> divPromise;
  std::future<int>  divResult = divPromise.get_future();

  std::cout << "divResult.valid() : " << divResult.valid() << std::endl;

  Div div;

  std::thread divThread{div, std::move(divPromise), 20, 10};
  std::cout << "divResult.valid() : " << divResult.valid() << std::endl;

  std::shared_future<int> shatFut = divResult.share();

  std::cout << "divResult.valid() : " << divResult.valid() << std::endl;

  Requestor   req;
  std::thread t1{req, shatFut};
  std::thread t2{req, shatFut};
  std::thread t3{req, shatFut};
  std::thread t4{req, shatFut};

  divThread.join();
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}