#include "assert.hpp"

#include <cstdlib>  // abort

#include <boost/stacktrace.hpp>

#include "logging.hpp"

namespace boost {

void assertion_failed_msg(const char* expr,
                          const char* msg,
                          const char* /* function */,
                          const char* file,
                          const long line)

{
  tactile::Print(fmt::color::orange_red,
                 "{}:{} expression '{}' evaluated to false: {}\n{}",
                 file,
                 line,
                 expr,
                 msg ? msg : "N/A",
                 stacktrace::stacktrace{});
  std::abort();
}

}  // namespace boost