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

#include <ctime>
#include <iostream>
#include <thread>

#include <ef.gy/test-case.h>

#include <chronod/julian.h>
#include <chronod/unix.h>

using namespace chronod;

int testCommonJDvsUNIX(std::ostream &log) {
  const auto UNIXEpochInJD = 2440587.5;
  const auto april152017InJD = 2457858.5;

  auto epoch = UNIX<>(0);
  auto april152017 = UNIX<>(1492214400);

  if (std::abs(julian::date<>(epoch).value - UNIXEpochInJD) > 0.1) {
    return 1;
  }
  if (std::abs(julian::date<>(april152017).value - april152017InJD) > 0.1) {
    return 2;
  }

  return 0;
}

int testJulianDateNowIsUNIXTimeAligned(std::ostream &log) {
  long double minDeviation = 9999;
  long double maxDeviation = 0;

  for (auto i = 0; i < 200; i++) {
    auto s = UNIX<>::now();
    auto j = julian::date<>::now();
    auto r = UNIX<>(j);
    long double dev = s.value - r.value;

    minDeviation = std::min(minDeviation, std::abs(dev));
    maxDeviation = std::max(maxDeviation, std::abs(dev));
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  log << "minimum deviation (UNIX<>::now - julian::date<>::now): "
      << minDeviation << "\n";
  log << "maximum deviation (UNIX<>::now - julian::date<>::now): "
      << maxDeviation << "\n";

  if (maxDeviation > 0.1) {
    return 1;
  }

  return 0;
}

TEST_BATCH(testCommonJDvsUNIX, testJulianDateNowIsUNIXTimeAligned)
