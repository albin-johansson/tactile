// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <format>       // make_format_args, format_args
#include <string_view>  // string_view

#include "tactile/base/log/log_level.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/runtime/api.hpp"

namespace tactile {
namespace internal {

TACTILE_RUNTIME_API void log(LogLevel level, std::string_view fmt, std::format_args args);

}  // namespace internal

/**
 * Logs a message using the internal logger.
 *
 * \tparam Args The format argument types.
 *
 * \param level The severity of the message.
 * \param fmt   The format string.
 * \param args  The format arguments.
 */
template <typename... Args>
void log(const LogLevel level, const std::string_view fmt, const Args&... args)
{
  internal::log(level, fmt, std::make_format_args(args...));
}

}  // namespace tactile
