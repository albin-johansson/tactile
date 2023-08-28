// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/crash_handler.hpp"

#include <cstdlib>  // abort

#include <boost/stacktrace.hpp>
#include <fmt/ostream.h>
#include <spdlog/spdlog.h>

namespace tactile {

void on_terminate()
{
  try {
    spdlog::critical("Into exile I must go. Failed I have.\n{}",
                     fmt::streamed(boost::stacktrace::stacktrace {}));
  }
  catch (...) {
    // Not much we can do.
  }

  std::abort();
}

}  // namespace tactile
