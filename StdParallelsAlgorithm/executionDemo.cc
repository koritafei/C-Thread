#include <algorithm>
#include <cmath>
#include <execution>
#include <iostream>
#include <vector>

void shiff(std::vector<int> &v) {
  int size = v.size();
  int k    = 0;
  for (int i = 0; i < size; i++) {
    while ((k = random() % size) > i) {
      std::swap(v[i], v[k]);
    }
  }
}

int main() {
  std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9};
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << "\n" << std::endl;
  shiff(v);
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
  std::sort(v.begin(), v.end());
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << "\n" << std::endl;

  shiff(v);
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << std::endl;

  std::sort(std::execution::seq, v.begin(), v.end());
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << "\n" << std::endl;

  shiff(v);
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
  std::sort(std::execution::par, v.begin(), v.end());
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << "\n" << std::endl;

  shiff(v);
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << std::endl;
  std::sort(std::execution::par_unseq, v.begin(), v.end());
  for (auto &it : v) {
    std::cout << it << ' ';
  }
  std::cout << "\n" << std::endl;
}
