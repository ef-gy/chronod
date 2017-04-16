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

#if !defined(CHRONOD_JULIAN_H)
#define CHRONOD_JULIAN_H

#include <chrono>
#include <cmath>
#include <string>

#include <ef.gy/json.h>

#include <chronod/unix.h>

namespace chronod {
namespace julian {
template <class Q = long double>
class date {
 public:
  constexpr date(const Q &pValue) : value(pValue) {}
  constexpr date(const UNIX<Q> &unix) : value(convertFromUNIX(unix.value)) {}

  Q value;

  template <class clock = std::chrono::system_clock>
  static date now(void) {
    const auto t = clock::now().time_since_epoch();
    return date(convertFromUNIX(Q(t.count()) * Q(clock::period::num) /
                                Q(clock::period::den)));
  }

  operator efgy::json::json(void) const {
    efgy::json::json v;
    v.toObject();
    v("julian-date") = value;
    return v;
  }

  operator UNIX<Q>(void) const { return UNIX<Q>(convertToUNIX(value)); }

 protected:
  static constexpr const Q JDtoUNIXoffset = 2440587.5;
  static constexpr const Q secondsPerUNIXDay = 86400;

  static constexpr const Q convertFromUNIX(const Q &unix) {
    return unix / secondsPerUNIXDay + JDtoUNIXoffset;
  }

  static constexpr const Q convertToUNIX(const Q &jd) {
    return (jd - JDtoUNIXoffset) * secondsPerUNIXDay;
  }
};

template <class N = long long>
class day {
 public:
  constexpr day(const N &pValue) : value(pValue) {}

  template <class Q = long double>
  constexpr day(const date<Q> &date) : value(std::floor(date.value)) {}

  N value;

  template <class Q = long double, class clock = std::chrono::system_clock>
  static day now(void) {
    return day(date<Q>::template now<clock>());
  }

  operator efgy::json::json(void) const {
    efgy::json::json v;
    v.toObject();
    v("julian-day") = (long double)value;
    return v;
  }
};
}

template <class Q = long double>
std::string to_string(const julian::date<Q> &t) {
  return std::to_string(t.value);
}

template <class N = long long>
std::string to_string(const julian::day<N> &t) {
  return std::to_string(t.value);
}
}

#endif
