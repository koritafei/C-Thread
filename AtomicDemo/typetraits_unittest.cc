#include <iostream>
#include <type_traits>

struct A {};

struct B {
  B(const B &) {}
};

struct C {
  virtual void fn() {}
};

struct D {
  int m;
};

struct E {
  virtual void foo();
};

struct F : E {};

int main() {
  std::cout << std::boolalpha;
  std::cout << "is_trivially_copy_constructible:"
            << std::endl; // 检测是否为普通可复制构造
  std::cout << "int : " << std::is_trivially_copy_constructible<int>::value
            << std::endl;
  std::cout << "A : " << std::is_trivially_copy_constructible<A>::value
            << std::endl;
  std::cout << "B : " << std::is_trivially_copy_constructible<B>::value
            << std::endl;
  std::cout << "C : " << std::is_trivially_copy_constructible<C>::value
            << std::endl;

  // 若 T 为多态类（即声明或继承至少一个虚函数的非联合类），则提供等于 true
  // 的成员常量 value 。对于任何其他类型， value 为 false 。
  // 若 T 是非联合类类型，则 T 应为完整类型；否则行为未定义。
  //  添加 is_polymorphic 或 is_polymorphic_v(C++ 17 起)
  //  的特化的程序行为未定义。
  std::cout << "is_polymorphic:" << std::endl;
  std::cout << "int: " << std::is_polymorphic<int>::value << std::endl;
  std::cout << "A: " << std::is_polymorphic<A>::value << std::endl;
  std::cout << "B: " << std::is_polymorphic<B>::value << std::endl;
  std::cout << "C: " << std::is_polymorphic<C>::value << std::endl;

  return 0;
}