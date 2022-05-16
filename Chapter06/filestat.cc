/**
 * @file filestat.cc
 * @author koritafei (koritafei@gmail.com)
 * @brief 文件属性获取
 * @version 0.1
 * @date 2020-08-06
 *
 * @copyright Copyright (c) 2020
 *
 */

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main() {
  struct stat buf;
  int         ret = stat("/etc/hosts", &buf);
  printf("ret is %d\n", ret);
  printf("/etc/hosts file size = %ld\n", buf.st_size);
  printf("/etc/hosts nlink = %lu\n", buf.st_nlink);
  memset(&buf, 0, sizeof(buf));
  ret = stat("/etc/abc", &buf);
  printf("ret is %d\n", ret);
  printf("/etc/abc file size = %ld\n", buf.st_size);
  printf("/etc/wget nlink = %lu\n", buf.st_nlink);
}
