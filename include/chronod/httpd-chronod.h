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

#include <chronod/unix.h>
#include <cxxhttp/httpd.h>
#include <ef.gy/stream-json.h>

namespace chronod {
namespace httpd {
template <class transport>
static bool chronod(
    typename cxxhttp::net::http::server<transport>::session &session,
    std::smatch &m) {
  std::ostringstream os("");

  UNIX<> t = UNIX<>::now();

  const std::string &target = m[0];
  bool useJSON = (m[3] == ".json");
  cxxhttp::net::http::headers head = {};

  if (useJSON) {
    os << efgy::json::tag() << efgy::json::json(t);
    head["Content-Type"] = "application/json";
  } else {
    os << to_string(t);
    head["Content-Type"] = "text/plain";
  }

  session.reply(200, head, os.str());

  return true;
}

static const char *regex = "/(unix)/(now)?(\\.json)?";

static cxxhttp::httpd::servlet<asio::ip::tcp> TCP(regex,
                                                  chronod<asio::ip::tcp>);
static cxxhttp::httpd::servlet<asio::local::stream_protocol> UNIX(
    regex, chronod<asio::local::stream_protocol>);
}
}

#endif
