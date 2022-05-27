#include <iostream>
#include <mutex>
#include <thread>

class Base {
public:
  virtual void interface() {
    std::lock_guard<std::mutex> lock{_mut};
    std::cout << "Base Lock" << std::endl;
  }

private:
  std::mutex _mut;
};

class Derived : public Base {
  void interface() override {
    std::cout << "Derived without Lock" << std::endl;
  }
};

int main() {
  Base *bar = new Derived();
  bar->interface();

  Derived der;
  Base   &bar2 = der;
  bar2.interface();
}
