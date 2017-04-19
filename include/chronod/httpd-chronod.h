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

#if !defined(CHRONOD_HTTPD_CHRONOD_H)
#define CHRONOD_HTTPD_CHRONOD_H

#include <ef.gy/stream-json.h>

#include <cxxhttp/httpd.h>

#include <chronod/julian.h>
#include <chronod/unix.h>

namespace chronod {
namespace httpd {
template <class time>
static inline std::string to_string(const time &t,
                                    cxxhttp::headers &head,
                                    bool useJSON) {
  if (useJSON) {
    std::ostringstream os("");
    os << efgy::json::tag() << efgy::json::json(t);
    return os.str();
  }

  return chronod::to_string(t);
}

template <class transport>
static bool chronod(
    typename cxxhttp::net::http::server<transport>::session &session,
    std::smatch &m) {
  std::string reply;

  const std::string &target = m[1];
  bool useJSON = (session.negotiated["Content-Type"] == "application/json");
  cxxhttp::headers head = {};

  if (target == "unix") {
    const auto t = UNIX<>::now();
    reply = to_string(t, head, useJSON);
  } else if (target == "julian-date") {
    const auto t = julian::date<>::now();
    reply = to_string(t, head, useJSON);
  } else if (target == "julian-day") {
    const auto t = julian::day<>::now();
    reply = to_string(t, head, useJSON);
  }

  session.reply(200, head, reply);

  return true;
}

namespace servlet {
static const char *resource = "/(unix|julian-date|julian-day)/(now)?";
static const char *method = "GET";
static const char *accept = "text/plain, application/json;q=0.9";

using cxxhttp::httpd::servlet;
using asio::ip::tcp;
using asio::local::stream_protocol;

static servlet<tcp> TCP(resource, chronod<tcp>, method, {{"Accept", accept}});
static servlet<stream_protocol> UNIX(resource, chronod<stream_protocol>, method,
                                     {{"Accept", accept}});
}
}
}

#endif
