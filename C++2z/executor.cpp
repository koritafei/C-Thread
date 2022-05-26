#include <atomic>
#include <experimental/thread_pool>
#include <iostream>
#include <utility>

namespace execution = std::experimental::execution;
using std::experimental::static_thread_pool;
using std::experimental::executorsv1::future;

int main() {
  static_thread_pool pool{4};
  auto               ex = pool.executor();

  // One way, single
  ex.execute([] {
    std::cout << "We made it!" << std::endl;
  });
  std::cout << std::endl;
  // Two way, single
  future<int> f1 = ex.twoway_execute([] {
    return 42;
  });
  f1.wait();
  std::cout << "The result is : " << f1.get() << std::endl;

  std::cout << std::endl;
  // One way, bulk.
  ex.bulk_execute(
      [](int n, int& sha) {
        std::cout << "part " << n << ": "
                  << "shared: " << sha << "\n";
      },
      8,
      [] {
        return 0;
      });
  std::cout << std::endl;

  // Two way, bulk, void result
  future<void> f2 = ex.bulk_twoway_execute(
      [](int n, std::atomic<short>& m) {
        std::cout << "async part " << n;
        std::cout << " atom: " << m++ << std::endl;
      },
      8,
      [] {
      },
      [] {
        std::atomic<short> atom(0);
        return std::ref(atom);
      });
  f2.wait();
  std::cout << "bulk result available" << std::endl;
  std::cout << std::endl;

  // Two way, bulk, non-void result.
  future<double> f3 = ex.bulk_twoway_execute(
      [](int n, double&, int&) {
        std::cout << "async part " << n << " ";
        std::cout << std::this_thread::get_id() << std::endl;
      },
      8,
      [] {
        std::cout << "Result factory: " << std::this_thread::get_id()
                  << std::endl;
        return 123.456;
      },
      [] {
        std::cout << "Shared Parameter: " << std::this_thread::get_id()
                  << std::endl;
        return 0;
      });
  f3.wait();
  std::cout << "bulk result is " << f3.get() << std::endl;
}
