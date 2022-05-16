#include <iostream>
#include <mutex>
#include <thread>

class SingleTon {
public:
  SingleTon(const SingleTon &) = delete;
  SingleTon &operator=(const SingleTon &) = delete;

  static SingleTon *getInstance() {
    std::call_once(initInstanceFlag, initSingleTon);
    return instance;
  }

  static void initSingleTon() {
    instance = new SingleTon();
  }

  static void del_object() {
    std::call_once(destoryInstanceFlag, destryObject);
  }

  static void destryObject() {
    if (instance) {
      delete instance;
      instance = nullptr;
    }
  }

private:
  static std::once_flag initInstanceFlag;
  static std::once_flag destoryInstanceFlag;

  static SingleTon *instance;
  SingleTon()  = default;
  ~SingleTon() = default;
};

SingleTon     *SingleTon::instance = nullptr;
std::once_flag SingleTon::initInstanceFlag;
std::once_flag SingleTon::destoryInstanceFlag;

void threadRun() {
  std::cout << "Thread id : " << std::this_thread::get_id()
            << " ,SingleTon getInstance " << SingleTon::getInstance()
            << std::endl;
}

int main() {
  std::cout << "Thread id : " << std::this_thread::get_id()
            << " ,SingleTon getInstance " << SingleTon::getInstance()
            << std::endl;

  std::thread t1{threadRun};
  std::thread t2{threadRun};

  t1.join();
  t2.join();

  std::cout << "Thread id : " << std::this_thread::get_id()
            << " ,SingleTon getInstance " << SingleTon::getInstance()
            << std::endl;
  SingleTon::del_object();
}
