#include <iostream>
#include <memory>

class Base {
public:
  Base()  = default;
  ~Base() = default;

  virtual void print() {
    std::cout << "base print" << std::endl;
  }
};

class Dev : public Base {
public:
  Dev() = default;
  virtual ~Dev() {
  }

  virtual void print() override {
    std::cout << "dev print" << std::endl;
  }
};

std::shared_ptr<Base> GetHandler(std::string const &name = "") {
  if (name == "dev") {
    return std::make_shared<Dev>();
  }

  return std::make_shared<Base>();
}

int main() {
  auto handler = GetHandler("dev");
  handler->print();

  auto handler2 = GetHandler();

  handler2->print();
}
