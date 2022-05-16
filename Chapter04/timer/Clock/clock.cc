#include "clock.h"

using namespace inter_clock;

void Clock::adjust_now(int h, int m, int s) {
  _now.set(h, m, s);
}
void Clock::adjust_ring(int h, int m, int s) {
  _ring_timer.set(h, m, s);
}
void Clock::tick() {
  long int old = time(0);
  while (old == time(0)) {
  }

  _now.next();
}
void Clock::showtime() {
  std::cout << _now.get() << std::endl;
}
void Clock::run() {
  do {
    tick();
    showtime();
    if (_now.get() >= _ring_timer.get()) {
      for (int i = 0; i < 20; i++) {
        std::cout << "\a";
      }
      return;
    }
  } while (true);
}