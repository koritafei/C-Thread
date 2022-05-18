#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int main() {
  auto begin = std::chrono::system_clock::now();

  auto asyncLazy  = std::async(std::launch::deferred, [] {
    return std::chrono::system_clock::now();
  });
  auto asyncEager = std::async(std::launch::async, [] {
    return std::chrono::system_clock::now();
  });

  std::this_thread::sleep_for(std::chrono::seconds(1));

  auto lazyStart = asyncLazy.get() - begin;
  auto edgeStart = asyncEager.get() - begin;

  auto lazyDuration = std::chrono::duration<double>(lazyStart).count();
  auto edgeDuration = std::chrono::duration<double>(edgeStart).count();

  std::cout << "asyncLazy evaluated after : " << lazyDuration << " seconds."
            << std::endl;
  std::cout << "asyncEager  evaluated after : " << edgeDuration << " seconds."
            << std::endl;
}
