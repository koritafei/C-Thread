/**
 * @file fileopeartor.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief cpp文件操作,和向不存在的文件写入数据
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  int           ar[] = {65, 66, 67, 68, 69, 1134688, 71, 70, 73, 74};
  std::ofstream ofile; //①
  struct stat   buf;
  ofile.open("TEST.txt", std::ios::out); //②
  if (!ofile) {
    std::cerr << "Open File Fail." << std::endl;
    exit(1);
  }

  for (int t = 0; t < sizeof(ar) / sizeof(int); ++t) {
    std::cout << "write file\n";
    ofile << ar[t] << " ";
  }
  ofile.flush();
  std::cout << ofile.failbit << std::endl;
  sleep(60); // 不关闭ofile,删除txt文件，文件还会继续写入，造成数据丢失
  /*std::cout << "SLEEP\n";
  if (ofile.bad()) { // 判断流是否有问题(文件被删除等错误)
    ofile.close();
    std::cout << "file is not exists\n";
    return -1;
  }
  for (int t = 0; t < sizeof(ar) / sizeof(int); ++t) {
    std::cout << "write file\n";
    ofile << ar[t] << " "; //向文件内写入
  }
  ofile.flush();*/

  std::cout << ofile.failbit << std::endl;
  ofile.close();
}