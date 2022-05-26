#include <iostream>
#include <thread>

class NonCopyableClass {
public:
  NonCopyableClass() = default;

  NonCopyableClass(const NonCopyableClass &) = delete;
  NonCopyableClass &operator=(const NonCopyableClass &) = delete;

private:
};

void perConstReference(const NonCopyableClass &nonCopy) {
}

int main() {
  NonCopyableClass nonCopy;

  perConstReference(nonCopy);

  std::thread t{perConstReference, std::ref(nonCopy)};
  t.join();
}
