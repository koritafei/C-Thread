#include <iostream>
#include <mutex>
#include <shared_mutex>

template <typename LOCK>
class StrategizedLocking {
public:
  explicit StrategizedLocking(LOCK &l) : _lock(l) {
    _lock.lock();
  }

  ~StrategizedLocking() {
    _lock.unlock();
  }

private:
  LOCK &_lock;
};

struct NullObjectMutex {
  void lock() {
  }

  void unlock() {
  }
};

class NoLock {
public:
  void lock() {
    std::cout << "NoLock::Lock " << std::endl;
    _nullObjectMutex.lock();
  }

  void unlock() {
    std::cout << "NoLock::unlock " << std::endl;
    _nullObjectMutex.unlock();
  }

private:
  mutable NullObjectMutex _nullObjectMutex;
};

class ExclusiveLock {
public:
  void lock() {
    std::cout << "ExclusiveLock:lock " << std::endl;
    _mutex.lock();
  }

  void unlock() {
    std::cout << "ExclusiveLock::unlock " << std::endl;
    _mutex.unlock();
  }

private:
  mutable std::mutex _mutex;
};

class SharedLock {
public:
  void lock() {
    std::cout << "SharedLock::lock " << std::endl;
    _shared_mutex.lock_shared();
  }

  void unlock() {
    std::cout << "SharedLock::unlock " << std::endl;
    _shared_mutex.unlock_shared();
  }

private:
  mutable std::shared_mutex _shared_mutex;
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