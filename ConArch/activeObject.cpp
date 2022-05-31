#include <algorithm>
#include <deque>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <numeric>
#include <random>
#include <thread>
#include <utility>
#include <vector>

using std::async;
using std::boolalpha;
using std::cout;
using std::deque;
using std::distance;
using std::endl;
using std::find_if;
using std::for_each;
using std::future;
using std::lock_guard;
using std::make_move_iterator;
using std::make_pair;
using std::move;
using std::mt19937;
using std::mutex;
using std::packaged_task;
using std::pair;
using std::random_device;
using std::sort;
using std::thread;
using std::uniform_int_distribution;
using std::vector;

class IsPrime {
public:
  pair<bool, int> operator()(int i) {
    for (int j = 2; j * j <= i; j++) {
      if (0 == i % j) {
        return make_pair(false, i);
      }
    }
    return make_pair(true, i);
  }

private:
};

class ActiveObject {
public:
  future<pair<bool, int>> enqueueTask(int i) {
    IsPrime isPrime;

    packaged_task<pair<bool, int>(int)> newJob(isPrime);

    auto isPrimoseFuture = newJob.get_future();
    auto pair            = make_pair(std::move(newJob), i);
    {
      lock_guard<mutex> lock{activationListMutex};
      activationList.push_back(move(pair));
    }

    return isPrimoseFuture;
  }

  void run() {
    thread servant{[this] {
      while (!isEmpty()) {
        auto myTask = dequeueTask();
        myTask.first(myTask.second);
      }
    }};

    servant.join();
  }

private:
  pair<packaged_task<pair<bool, int>(int)>, int> dequeueTask() {
    lock_guard<mutex> lock{activationListMutex};

    auto myTask = std::move(activationList.front());
    activationList.pop_front();

    return myTask;
  }

  bool isEmpty() {
    lock_guard<mutex> lock{activationListMutex};

    auto empty = activationList.empty();
    return empty;
  }

  deque<pair<packaged_task<pair<bool, int>(int)>, int>> activationList;

  mutex activationListMutex;
};

vector<int> getRandomNumber(int number) {
  random_device              seed;
  mt19937                    engine(seed());
  uniform_int_distribution<> dist(1000000, 1000000000);
  vector<int>                numbers;
  for (long long i = 0; i < number; i++) {
    numbers.push_back(dist(engine));
  }

  return numbers;
}

future<vector<future<pair<bool, int>>>> getFutures(ActiveObject &activeObject,
                                                   int           numberPrimes) {
  return async([&activeObject, numberPrimes] {
    vector<future<pair<bool, int>>> futures;
    auto randomNumber = getRandomNumber(numberPrimes);
    for (auto numb : randomNumber) {
      futures.push_back(activeObject.enqueueTask(numb));
    }

    return futures;
  });
}

int main() {
  cout << boolalpha << endl;
  ActiveObject activeObject;

  auto client1 = getFutures(activeObject, 1998);
  auto client2 = getFutures(activeObject, 2003);
  auto client3 = getFutures(activeObject, 2011);
  auto client4 = getFutures(activeObject, 2014);
  auto client5 = getFutures(activeObject, 2017);

  auto futures  = client1.get();
  auto futures2 = client2.get();
  auto futures3 = client3.get();
  auto futures4 = client4.get();
  auto futures5 = client5.get();

  futures.insert(futures.end(),
                 make_move_iterator(futures2.end()),
                 make_move_iterator(futures2.end()));
  futures.insert(futures.end(),
                 make_move_iterator(futures3.end()),
                 make_move_iterator(futures3.end()));
  futures.insert(futures.end(),
                 make_move_iterator(futures4.end()),
                 make_move_iterator(futures4.end()));
  futures.insert(futures.end(),
                 make_move_iterator(futures5.end()),
                 make_move_iterator(futures5.end()));

  activeObject.run();
  vector<pair<bool, int>> futResults;
  futResults.reserve(futResults.size());

  for (auto &fut : futures) {
    futResults.push_back(fut.get());
  }

  sort(futResults.begin(), futResults.end());

  auto prIt =
      find_if(futResults.begin(), futResults.end(), [](pair<bool, int> pa) {
        return pa.first == true;
      });

  cout << "Number primes: " << distance(prIt, futResults.end()) << endl;
  cout << "Primes: " << endl;

  for_each(prIt, futResults.end(), [](pair<bool, int> p) {
    cout << p.second << ' ';
  });
  cout << "\n\n";

  cout << "Number no primes: " << distance(futResults.begin(), prIt) << endl;
  cout << "No primes: " << endl;
  for_each(futResults.begin(), prIt, [](pair<bool, int> p) {
    cout << p.second << " ";
  });
  cout << endl;
}