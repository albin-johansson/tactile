// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#pragma once

#include <format>

#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/log/log_category.hpp"
#include "tactile/core/log/log_sink.hpp"
#include "tactile/core/util/format_buffer.hpp"
#include "tactile/core/util/time.hpp"

namespace tactile {

/// A sink based logger implementation.
class Logger final
{
 public:
  using clock_type = SteadyClock;

  /// Adds a sink to logger.
  void add_sink(Unique<ILogSink> sink);

  /// Sets the reference time used to calculate timestamps.
  void set_ref_instant(clock_type::time_point instant) noexcept;

  /// Sets the minimum category for messages to be logged by `log`.
  void set_min_category(LogCategory category) noexcept;

  /// Sets the minimum category for messages to be flushed by `log`.
  void set_flush_category(LogCategory category) noexcept;

  /// Indicates whether a message with a given category would be logged by `log`.
  auto would_log(LogCategory category) const noexcept -> bool;

  /// Logs a message if the given category is severe enough.
  template <typename... Args>
  void log(const LogCategory category,
           const std::format_string<Args...> fmt,
           const Args&... args) noexcept
  {
    if (would_log(category)) {
      log_impl(category, fmt.get(), std::make_format_args(args...));
    }
  }

 private:
  clock_type::time_point m_ref_instant {};
  LogCategory m_min_category {LogCategory::kInfo};
  LogCategory m_flush_category {LogCategory::kError};
  Vector<Unique<ILogSink>> m_sinks {};
  FormatBuffer<32> m_prefix_buffer {};
  FormatBuffer<512> m_message_buffer {};

  void log_impl(LogCategory category,
                StringView fmt,
                const std::format_args& args) noexcept;
};

/// Sets the global logger.
void set_logger(Logger* logger) noexcept;

/// Returns a pointer to the global logger, or null if there is none.
auto get_logger() noexcept -> Logger*;

/// Attempts to log a message using the global logger.
template <typename... Args>
void log(const LogCategory category,
         const std::format_string<Args...> fmt,
         const Args&... args) noexcept
{
  if (auto* const logger = get_logger()) {
    logger->log(category, fmt, args...);
  }
}

/// Attempts to log a verbose message using the global logger.
template <typename... Args>
void log_verbose(const std::format_string<Args...> fmt, const Args&... args) noexcept
{
  log(LogCategory::kVerbose, fmt, args...);
}

/// Attempts to log a debug message using the global logger.
template <typename... Args>
void log_debug(const std::format_string<Args...> fmt, const Args&... args) noexcept
{
  log(LogCategory::kDebug, fmt, args...);
}

/// Attempts to log an informative message using the global logger.
template <typename... Args>
void log_info(const std::format_string<Args...> fmt, const Args&... args) noexcept
{
  log(LogCategory::kInfo, fmt, args...);
}

/// Attempts to log a warning message using the global logger.
template <typename... Args>
void log_warn(const std::format_string<Args...> fmt, const Args&... args) noexcept
{
  log(LogCategory::kWarn, fmt, args...);
}

/// Attempts to log an error message using the global logger.
template <typename... Args>
void log_error(const std::format_string<Args...> fmt, const Args&... args) noexcept
{
  log(LogCategory::kError, fmt, args...);
}

}  // namespace tactile
