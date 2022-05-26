#include "clock.h"

#include <chrono>

int64_t Clock::CurrentTimeMillis() {
  auto now = std::chrono::steady_clock::now();
  return std::chrono::duration_cast<std::chrono::milliseconds>(
           now.time_since_epoch())
    .count();
}

std::string Clock::CurrentTimeString(int64_t timepoint) {
  time_t     sec         = timepoint / 1000;
  struct tm* pinfo       = localtime(&sec);
  char       buffer[100] = {};
  size_t     n = snprintf(buffer, 100, "%d-%02d-%02d %02d:%02d:%02d.%03d",
                      1900 + pinfo->tm_year, pinfo->tm_mon + 1, pinfo->tm_mday,
                      pinfo->tm_hour, pinfo->tm_min, pinfo->tm_sec,
                      int(timepoint % 1000));
  return std::string(buffer, n);
}

uint64_t Clock::GetNowTickCount() {
  timespec ts;
  if (0 > clock_gettime(CLOCK_MONOTONIC, &ts)) {
    return 0;
  }

  return (ts.tv_sec * 1000000000UL) + ts.tv_nsec;
}

int64_t Clock::CurrentTimeUnits() {
  return CurrentTimeMillis() / TIME_UINT;
}
