// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/assert.hpp"

#include <exception>  // terminate

#include <spdlog/spdlog.h>

namespace tactile {

void on_assertion_failed(const char* expr,
                         const char* msg,
                         const char* file,
                         const int64 line)
{
  spdlog::critical("{}:{} expression '{}' is false: {}",
                   file,
                   line,
                   expr,
                   msg ? msg : "?");
  std::terminate();
}

}  // namespace tactile
