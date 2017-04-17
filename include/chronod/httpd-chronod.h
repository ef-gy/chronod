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
static inline std::string flatten(const time &t,
                                  cxxhttp::net::http::headers &head,
                                  bool useJSON) {
  if (useJSON) {
    std::ostringstream os("");
    os << efgy::json::tag() << efgy::json::json(t);
    return os.str();
  }

  return to_string(t);
}

template <class transport>
static bool chronod(
    typename cxxhttp::net::http::server<transport>::session &session,
    std::smatch &m) {
  std::string reply;

  const std::string &target = m[1];
  bool useJSON = (session.negotiated["Content-Type"] == "application/json");
  cxxhttp::net::http::headers head = {};

  if (target == "unix") {
    const auto t = UNIX<>::now();
    reply = flatten(t, head, useJSON);
  } else if (target == "julian-date") {
    const auto t = julian::date<>::now();
    reply = flatten(t, head, useJSON);
  } else if (target == "julian-day") {
    const auto t = julian::day<>::now();
    reply = flatten(t, head, useJSON);
  }

  session.reply(200, head, reply);

  return true;
}

static const char *regex = "/(unix|julian-date|julian-day)/(now)?";

static cxxhttp::httpd::servlet<asio::ip::tcp> TCP(
    regex, chronod<asio::ip::tcp>, "GET",
    {{"Accept", "text/plain, application/json;q=0.9"}});
static cxxhttp::httpd::servlet<asio::local::stream_protocol> UNIX(
    regex, chronod<asio::local::stream_protocol>, "GET",
    {{"Accept", "text/plain, application/json;q=0.9"}});
}
}

#endif
