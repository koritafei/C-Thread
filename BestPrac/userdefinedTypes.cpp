#include <iostream>
#include <ostream>

class MyInt {
public:
  constexpr MyInt() = default;
  constexpr MyInt(int first, int second) : myVal1(first), myVal2(second) {
  }

  MyInt(int i) {
    myVal1 = i - 2;
    myVal2 = i + 3;
  }

  constexpr int getSum() const {
    return myVal1 + myVal2;
  }

  friend std::ostream &operator<<(std::ostream &out, const MyInt myInt) {
    out << "(" << myInt.myVal1 << "," << myInt.myVal2 << ")";
    return out;
  }

private:
  int myVal1 = 1998;
  int myVal2 = 2003;
};

int main() {
  constexpr MyInt myIntConst1;
  constexpr int   sec = 2014;
  constexpr MyInt myIntConst2(2011, sec);
  std::cout << "myIntConst2.getSum(): " << myIntConst2.getSum() << std::endl;
  int arr[myIntConst2.getSum()];
  static_assert(myIntConst2.getSum() == 4025, "2011 + 2014 should be 4025");
}
