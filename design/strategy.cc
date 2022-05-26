#include <iostream>
#include <memory>

class Strategy {
public:
  typedef std::shared_ptr<Strategy> ptr;

  virtual void operator()() = 0;
  virtual ~Strategy()       = default;

private:
};

class Context {
public:
  explicit Context() : _start(nullptr) {
  }

  void setStrategy(Strategy::ptr ptr) {
    _start = ptr;
  }

  void stratefy() {
    if (nullptr != _start) {
      (*_start)();
    }
  }

private:
  Strategy::ptr _start;
};

class Strategy1 : public Strategy {
  void operator()() override {
    std::cout << "Foo" << std::endl;
  }
};

class Strategy2 : public Strategy {
  void operator()() override {
    std::cout << "Bar" << std::endl;
  }
};

class Strategy3 : public Strategy {
  void operator()() override {
    std::cout << "FooBar" << std::endl;
  }
};

int main() {
  Context con;
  con.setStrategy(std::make_shared<Strategy1>());
  con.stratefy();

  con.setStrategy(std::make_shared<Strategy2>());
  con.stratefy();

  con.setStrategy(std::make_shared<Strategy3>());
  con.stratefy();
}