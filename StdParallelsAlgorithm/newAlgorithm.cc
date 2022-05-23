#include <algorithm>
#include <execution>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

int main() {
  std::vector<int> intVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  std::for_each_n(std::execution::par, intVec.begin(), 5, [](int &arg) {
    arg *= arg;
  });

  std::cout << "std::for_each_n : ";
  for (auto iter : intVec) {
    std::cout << iter << ' ';
  }

  std::cout << "\n\n";

  std::vector<int> resVec{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::exclusive_scan(std::execution::par,
                      resVec.begin(),
                      resVec.end(),
                      resVec.begin(),
                      1,
                      [](int first, int second) {
                        return first * second;
                      });
  std::cout << "std::exclusive_scan : ";
  for (auto iter : resVec) {
    std::cout << iter << ' ';
  }

  std::cout << "\n\n";

  std::vector<int> resVec2{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::inclusive_scan(std::execution::par,
                      resVec2.begin(),
                      resVec2.end(),
                      resVec2.begin(),
                      [](int first, int second) {
                        return first * second;
                      });
  std::cout << "std::inclusive_scan : ";
  for (auto iter : resVec2) {
    std::cout << iter << ' ';
  }

  std::cout << "\n\n";

  std::vector<int> resVec3{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::vector<int> resVec4(resVec3.size());
  std::transform_exclusive_scan(
      std::execution::par,
      resVec3.begin(),
      resVec3.end(),
      resVec4.begin(),
      0,
      [](int first, int second) {
        return first + second;
      },
      [](int arg) {
        arg *= arg;
      });
  std::cout << "std::transform_exclusive_scan : ";
  for (auto iter : resVec4) {
    std::cout << iter << ' ';
  }

  std::cout << "\n\n";

  std::vector<std::string> strVec{"Only", "for", "testing", "purpose"};
  std::vector<int>         resVec5(strVec.size());
  std::transform_inclusive_scan(
      std::execution::par,
      strVec.begin(),
      strVec.end(),
      resVec5.begin(),
      0,
      [](auto first, auto second) {
        return first + second;
      },
      [](auto s) {
        return s.length();
      });
  std::cout << "std::transform_inclusive_scan : ";
  for (auto iter : resVec5) {
    std::cout << iter << ' ';
  }

  std::cout << "\n\n";

  std::vector<std::string> strVec2{"Only", "for", "testing", "purpose"};
  std::string              res = std::reduce(std::execution::par,
                                strVec2.begin() + 1,
                                strVec2.end(),
                                strVec2[0],
                                [](auto fir, auto sec) {
                                  return fir + ":" + sec;
                                });
  std::cout << "reduce : " << res << "\n\n";

  std::size_t res7 = std::transform_reduce(
      std::execution::par,
      strVec2.begin(),
      strVec2.end(),
      [](std::string s) {
        return s.length();
      },
      0,
      [](std::size_t a, std::size_t b) {
        return a + b;
      });

  std::cout << "transform_reduce : " << res7 << "\n\n";
}
