/**\file
 *
 * \copyright
 * This file is part of the chronod project, which is released as open source
 * under the terms of an MIT/X11-style licence, described in the COPYING file.
 *
 * \see Documentation: https://ef.gy/documentation/chronod
 * \see Source Code: https://github.com/ef-gy/chronod
 * \see Licence Terms: https://github.com/ef-gy/chronod/COPYING
 */

#include <iostream>

#include <ef.gy/test-case.h>

#include <chronod/unix.h>
#include <ctime>
#include <thread>

using namespace chronod;

int testIntegerUNIXNowIsUNIXTime(std::ostream &log) {
  long double minDeviation = 9999;
  long double maxDeviation = 0;

  for (auto i = 0; i < 200; i++) {
    auto s = UNIX<long long>::now().value;
    auto r = std::time(0);
    long double dev = s - r;

    minDeviation = std::min(minDeviation, std::abs(dev));
    maxDeviation = std::max(maxDeviation, std::abs(dev));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  log << "(max, min) deviation (UNIX<long long>::now - std::time): ("
      << minDeviation << ", " << maxDeviation << "): ";

  if (maxDeviation > 2.) {
    return 1;
  }

  return 0;
}

int testUNIXNowIsUNIXTime(std::ostream &log) {
  long double minDeviation = 9999;
  long double maxDeviation = 0;

  for (auto i = 0; i < 200; i++) {
    auto s = UNIX<>::now().value;
    auto r = std::time(0);
    long double dev = s - r;

    minDeviation = std::min(minDeviation, std::abs(dev));
    maxDeviation = std::max(maxDeviation, std::abs(dev));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  log << "(max, min) deviation (UNIX<>::now - std::time): (" << minDeviation
      << ", " << maxDeviation << "): ";

  if (maxDeviation >= 1.1) {
    return 1;
  }

  return 0;
}

TEST_BATCH(testIntegerUNIXNowIsUNIXTime, testUNIXNowIsUNIXTime)
