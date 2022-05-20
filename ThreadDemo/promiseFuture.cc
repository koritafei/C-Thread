#include <future>
#include <iostream>
#include <thread>
#include <utility>

void product(std::promise<int> &&intPromise, int a, int b) {
  intPromise.set_value(a * b);
}

struct Div {
  void operator()(std::promise<int> &&intPromise, int a, int b) const {
    intPromise.set_value(a / b);
  }
};

int main() {
  int a = 20;
  int b = 10;

  // define promise
  std::promise<int> prodPromise;
  std::promise<int> divPromise;

  // get the future
  std::future<int> prodFuture = prodPromise.get_future();
  std::future<int> divFuture  = divPromise.get_future();

  // calculate the result in a separate thread
  std::thread prodThread{product, std::move(prodPromise), a, b};

  Div         div;
  std::thread divThread{div, std::move(divPromise), a, b};

  // get the result
  std::cout << "20 * 10 = " << prodFuture.get() << std::endl;
  std::cout << "20 / 10 = " << divFuture.get() << std::endl;

  prodThread.join();
  divThread.join();

  return 0;
}