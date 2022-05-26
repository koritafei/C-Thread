/**
 * @file async_paramers.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 使用async传递参数
 * @version 0.1
 * @date 2020-06-25
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <future>
#include <iostream>
#include <string>
#include <functional>
#include <algorithm>

struct X {
    void foo(int i, std::string const &str) {
        std::cout << "integer is " << i << " , string is " << str << std::endl;
    }

    std::string bar(std::string const &str) {
        return "Return string " + str;
    }
};

X    x;
auto f1 = std::async(&X::foo, &x, 42, "hello");
auto f2 = std::async(&X::bar, x, "good byte");

struct Y {
    double operator()(double d) {
        return d * d;
    }
};

Y    y;
auto f3 = std::async(Y(), 3.14);
auto f4 = std::async(std::ref(y), 2.17);

int main(int argc, char **argv) {
    f1.get();
    std::cout << f2.get() << std::endl;
    std::cout << f3.get() << std::endl;
    std::cout << f4.get() << std::endl;
}
