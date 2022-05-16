#include "clock.h"

int main(int argc, char **argv) {
  inter_clock::Clock c;
  c.adjust_now(12, 35, 40);   //起始时间
  c.adjust_ring(12, 36, 45);  //闹铃时间

  c.run();
}
