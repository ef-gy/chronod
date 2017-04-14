/**\file
 * \brief chronod Server
 *
 * Call it like this:
 * \code
 * $ ./chronod http:localhost:8080
 * \endcode
 *
 * \copyright
 * This file is part of the chronod project, which is released as open source
 * under the terms of an MIT/X11-style licence, described in the COPYING file.
 *
 * \see Documentation: https://ef.gy/documentation/chronod
 * \see Source Code: https://github.com/ef-gy/chronod
 * \see Licence Terms: https://github.com/ef-gy/chronod/COPYING
 */

#define ASIO_DISABLE_THREADS
#include <cxxhttp/httpd.h>

#include <chronod/httpd-chronod.h>

// Optional server features.
#include <cxxhttp/httpd-options.h>
#include <cxxhttp/httpd-quit.h>
#include <cxxhttp/httpd-trace.h>

int main(int argc, char *argv[]) { return cxxhttp::io::main(argc, argv); }
