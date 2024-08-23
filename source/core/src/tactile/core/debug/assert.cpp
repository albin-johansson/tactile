// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/assert.hpp"

#include <exception>  // terminate

#include "tactile/core/log/logger.hpp"

namespace tactile {

void on_assertion_failed(const char* expr, const char* msg, const char* file, const int line)
{
  TACTILE_LOG_FATAL("{}:{} expression '{}' is false: {}", file, line, expr, msg ? msg : "?");
  std::terminate();
}

}  // namespace tactile
