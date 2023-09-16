// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/terminate_handler.hpp"

#include <cstdlib>  // abort

#include <boost/stacktrace.hpp>
#include <fmt/ostream.h>

#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

void on_terminate()
{
  try {
    TACTILE_LOG_FATAL("Into exile I must go. Failed I have.\n{}",
                      fmt::streamed(boost::stacktrace::stacktrace {}));
  }
  catch (...) {
    // Not much we can do.
  }

  std::abort();
}

}  // namespace tactile
