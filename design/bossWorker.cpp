#include <chrono>
#include <future>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <utility>

int getRandomTime(int start, int end) {
  std::random_device seed;
  std::mt19937       engine{seed()};

  std::uniform_int_distribution<int> dist{start, end};

  return dist(engine);
}

class Worker {
public:
  explicit Worker(const std::string &name) : _name(name) {
  }

  void operator()(std::promise<void>     &&prepareWork,
                  std::shared_future<void> boos2Worker) {
    int prepareTime = getRandomTime(500, 2000);
    std::this_thread::sleep_for(std::chrono::microseconds(prepareTime));

    prepareWork.set_value();
    std::cout << _name << ": "
              << "Work prepared after " << prepareTime << " milliseconds."
              << std::endl;

    boos2Worker.wait();
  }

private:
  std::string _name;
};

int main() {
  std::promise<void>       startWorkPromise;
  std::shared_future<void> startWorkFuture = startWorkPromise.get_future();

  std::promise<void> herbPrepare;
  std::future<void>  waitForHerb = herbPrepare.get_future();

  Worker      herb{"herb"};
  std::thread herbWork{herb, std::move(herbPrepare), startWorkFuture};

  std::promise<void> scottPrepare;
  std::future<void>  waitForScott = scottPrepare.get_future();

  Worker      scott{"scott"};
  std::thread scottWork{scott, std::move(scottPrepare), startWorkFuture};

  std::promise<void> bjarnePrepare;
  std::future<void>  waitForBjarne = bjarnePrepare.get_future();

  Worker      bjarne{"bjarne"};
  std::thread bjarneWork{bjarne, std::move(bjarnePrepare), startWorkFuture};

  std::cout << "BOSS: PREPARE YOUR WORK.\n " << std::endl;

  waitForHerb.wait(), waitForScott.wait(), waitForBjarne.wait();
  std::cout << "\nBOSS: START YOUR WORK. \n" << std::endl;

  startWorkPromise.set_value();

  herbWork.join();
  scottWork.join();
  bjarneWork.join();
}
