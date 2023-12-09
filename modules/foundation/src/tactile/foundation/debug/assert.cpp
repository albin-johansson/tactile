// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/debug/assert.hpp"

#include <exception>  // terminate

#include "tactile/foundation/log/logger.hpp"

namespace tactile {

void on_assertion_failed(const char* expr,
                         const char* msg,
                         const char* file,
                         const int64 line)
{
  TACTILE_LOG_FATAL("{}:{} expression '{}' is false: {}",
                    file,
                    line,
                    expr,
                    msg ? msg : "?");
  std::terminate();
}

}  // namespace tactile
