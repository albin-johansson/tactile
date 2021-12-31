#include "assert.hpp"

#include <cstdlib>  // abort

#include <boost/stacktrace.hpp>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/ostream.h>

namespace boost {

void assertion_failed_msg(const char* expr,
                          const char* msg,
                          const char* /* function */,
                          const char* file,
                          const long line)

{
  if constexpr (Tactile::IsPlatformWindows()) {
    fmt::print("{}:{} expression '{}' evaluated to false: {}\n{}",
               file,
               line,
               expr,
               msg ? msg : "N/A",
               stacktrace::stacktrace{});
  }
  else {
    fmt::print(fmt::fg(fmt::color::orange_red),
               "{}:{} expression '{}' evaluated to false: {}\n{}",
               file,
               line,
               expr,
               msg ? msg : "N/A",
               stacktrace::stacktrace{});
  }

  std::abort();
}

}  // namespace boost