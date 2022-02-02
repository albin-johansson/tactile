#include "assert.hpp"

#include <cstdlib>  // abort

#include <boost/stacktrace.hpp>

#include "crash.hpp"
#include "logging.hpp"

namespace boost {

void assertion_failed_msg(const char* expr,
                          const char* msg,
                          const char* /* function */,
                          const char* file,
                          const long line)

{
  const stacktrace::stacktrace trace{};

  tactile::print(fmt::color::orange_red,
                 "{}:{} expression '{}' evaluated to false: {}\n{}",
                 file,
                 line,
                 expr,
                 msg ? msg : "N/A",
                 trace);
  tactile::dump_crash_info(trace);

  std::abort();
}

}  // namespace boost