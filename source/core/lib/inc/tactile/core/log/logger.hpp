// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/format.h>

#include "tactile/core/api.hpp"
#include "tactile/core/basic/clock.hpp"
#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/smart_ptr.hpp"
#include "tactile/core/basic/string.hpp"
#include "tactile/core/log/log_level.hpp"
#include "tactile/core/log/log_sink.hpp"

namespace tactile {

/// A simple sink-based logger implementation.
class TACTILE_CORE_API Logger final
{
 public:
  using clock_type = SteadyClock;

  TACTILE_DELETE_COPY(Logger);

  Logger();

  Logger(Logger&&) noexcept;

  ~Logger() noexcept;

  auto operator=(Logger&&) noexcept -> Logger&;

  /// Logs a message.
  template <typename... Args>
  void log(const LogLevel level, const StringView fmt, const Args&... args) noexcept
  {
    _log(level, fmt, fmt::make_format_args(args...));
  }

  /// Adds a sink to the logger.
  void add_sink(std::unique_ptr<ILogSink> sink);

  /// Sets a reference time point to use as a relative baseline for timestamps.
  void set_reference_instant(clock_type::time_point instant);

  /// Sets the minimum severity of messages that get logged.
  void set_log_level(LogLevel level);

  /// Sets the minimum severity of messages that will initiate flushes.
  void set_flush_level(LogLevel level);

  /// Indicates whether a message with a given severity would be logged.
  [[nodiscard]]
  auto would_log(LogLevel level) const noexcept -> bool;

  /// Indicates whether a message with a given severity would trigger a flush.
  [[nodiscard]]
  auto would_flush(LogLevel level) const noexcept -> bool;

 private:
  struct Data;
  Unique<Data> m_data;

  void _log(LogLevel level, StringView fmt, fmt::format_args args) noexcept;
};

/// Sets the global core logger.
TACTILE_CORE_API void set_logger(Logger* logger) noexcept;

/// Returns the global core logger, if any.
[[nodiscard]]
TACTILE_CORE_API auto get_logger() noexcept -> Logger*;

}  // namespace tactile

#define TACTILE_LOG(Lvl, Fmt, ...)                                        \
  do {                                                                    \
    const ::tactile::LogLevel _tactile_log_macro_level = (Lvl);           \
    auto* const _tactile_log_macro_logger = ::tactile::get_logger();      \
    if (_tactile_log_macro_logger &&                                      \
        _tactile_log_macro_logger->would_log(_tactile_log_macro_level)) { \
      _tactile_log_macro_logger->log(_tactile_log_macro_level,            \
                                     (Fmt) __VA_OPT__(, ) __VA_ARGS__);   \
    }                                                                     \
  } while (false)

#define TACTILE_LOG_TRC(Fmt, ...) TACTILE_LOG(::tactile::LogLevel::kTrace, Fmt, __VA_ARGS__)
#define TACTILE_LOG_DBG(Fmt, ...) TACTILE_LOG(::tactile::LogLevel::kDebug, Fmt, __VA_ARGS__)
#define TACTILE_LOG_INF(Fmt, ...) TACTILE_LOG(::tactile::LogLevel::kInfo, Fmt, __VA_ARGS__)
#define TACTILE_LOG_WRN(Fmt, ...) TACTILE_LOG(::tactile::LogLevel::kWarn, Fmt, __VA_ARGS__)
#define TACTILE_LOG_ERR(Fmt, ...) TACTILE_LOG(::tactile::LogLevel::kError, Fmt, __VA_ARGS__)
