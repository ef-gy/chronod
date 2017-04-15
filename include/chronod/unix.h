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

#if !defined(CHRONOD_UNIX_H)
#define CHRONOD_UNIX_H

#include <chrono>
#include <cmath>
#include <string>

#include <ef.gy/json.h>

namespace chronod {
template <class Q = long double>
class UNIX {
 public:
  constexpr UNIX(const Q &pValue) : value(pValue) {}

  Q value;

  template <class clock = std::chrono::system_clock>
  static UNIX now(void) {
    const auto t = clock::now().time_since_epoch();
    return UNIX(Q(t.count()) * Q(clock::period::num) / Q(clock::period::den));
  }

  operator efgy::json::json(void) const {
    efgy::json::json v;
    v.toObject();
    v("unix") = value;
    return v;
  }
};

template <class Q = long double>
std::string to_string(const UNIX<Q> &t) {
  return std::to_string(t.value);
}
}

#endif
