#include <iostream>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <thread>

class Lock {
public:
  typedef std::shared_ptr<Lock> ptr;

  virtual void lock() const   = 0;
  virtual void unlock() const = 0;

private:
};

class StrategizedLocking {
public:
  typedef std::shared_ptr<StrategizedLocking> ptr;

  explicit StrategizedLocking(Lock &l) : _lock(l) {
    _lock.lock();
  }

  ~StrategizedLocking() {
    _lock.unlock();
  }

private:
  Lock &_lock;
};

struct NullObjectMutex {
  void lock(){};
  void unlock(){};
};

class NoLock : public Lock {
  void lock() const override {
    std::cout << "NoLock::lock" << std::endl;
    nullObjetcMutex.lock();
  }

  void unlock() const override {
    std::cout << "NoLock unlock" << std::endl;
    nullObjetcMutex.unlock();
  }

  mutable NullObjectMutex nullObjetcMutex;
};

class ExclusiveLock : public Lock {
  void lock() const override {
    std::cout << "ExclusiveLock::lock" << std::endl;
    mutex.lock();
  }

  void unlock() const override {
    std::cout << "ExclusiveLock unlock" << std::endl;
    mutex.unlock();
  }

  mutable std::mutex mutex;
};

class SharedLock : public Lock {
  void lock() const override {
    std::cout << "SharedLock::lock" << std::endl;
    sharedMutex.lock_shared();
  }

  void unlock() const override {
    std::cout << "SharedLock unlock" << std::endl;
    sharedMutex.unlock_shared();
  }

  mutable std::shared_mutex sharedMutex;
};

int main() {
  NoLock             noLock;
  StrategizedLocking stratLock1{noLock};
  {
    ExclusiveLock      exLock;
    StrategizedLocking stratLock2{exLock};
    {
      SharedLock         sharLock;
      StrategizedLocking startLock3{sharLock};
    }
  }
}