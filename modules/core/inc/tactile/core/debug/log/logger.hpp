// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/core.h>

#include "tactile/core/api.hpp"
#include "tactile/core/debug/log/log_level.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"
#include "tactile/core/type/maybe.hpp"
#include "tactile/core/type/string.hpp"
#include "tactile/core/type/vector.hpp"

#define TACTILE_LOG(Level, FmtString, ...)                                           \
  do {                                                                               \
    auto* tactile_logger = tactile::get_logger();                                    \
    if (tactile_logger && tactile_logger->would_log((Level))) {                      \
      tactile_logger->log((Level), (FmtString), fmt::make_format_args(__VA_ARGS__)); \
    }                                                                                \
  } while (false)

#define TACTILE_LOG_TRACE(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kTrace, FmtString __VA_OPT__(, ) __VA_ARGS__)

#define TACTILE_LOG_DEBUG(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kDebug, FmtString __VA_OPT__(, ) __VA_ARGS__)

#define TACTILE_LOG_INFO(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kInfo, FmtString __VA_OPT__(, ) __VA_ARGS__)

#define TACTILE_LOG_WARN(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kWarn, FmtString __VA_OPT__(, ) __VA_ARGS__)

#define TACTILE_LOG_ERROR(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kError, FmtString __VA_OPT__(, ) __VA_ARGS__)

#define TACTILE_LOG_FATAL(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kFatal, FmtString __VA_OPT__(, ) __VA_ARGS__)

namespace tactile {

TACTILE_FWD(class ILoggerSink)

/** A simple logger implementation. */
class TACTILE_CORE_API Logger final {
 public:
  /** Logs an arbitrary message. */
  void log(LogLevel level, StringView fmt_string, fmt::format_args args) noexcept;

  /** Sets the minimum severity of messages that get logged. */
  void set_min_level(LogLevel level) noexcept;

  /** Sets the minimum severity of messages that will initiate flushes. */
  void flush_on(LogLevel level) noexcept;

  /** Associates a sink implementation with the logger. */
  void add_sink(ILoggerSink* sink);

  /** Sets a reference time point to use as a relative baseline for timestamps. */
  void set_reference_instant(Maybe<SteadyClockInstant> instant);

  /** Indicates whether a message with the specified severity would be logged. */
  [[nodiscard]] auto would_log(LogLevel level) const noexcept -> bool;

  /** Indicates whether a message with the specified severity would trigger a flush. */
  [[nodiscard]] auto would_flush(LogLevel level) const noexcept -> bool;

  /** Returns a three-letter acronym for the specified log level. */
  [[nodiscard]] static auto get_acronym(LogLevel level) noexcept -> StringView;

 private:
  LogLevel mMinLevel {LogLevel::kInfo};
  LogLevel mFlushLevel {LogLevel::kError};
  Vector<ILoggerSink*> mSinks;
  Maybe<SteadyClockInstant> mReferenceInstant;

  [[nodiscard]] auto _to_elapsed_time(SteadyClockInstant instant) const -> Microseconds;
};

/** Sets the logger instance that is used by the logging macros. */
TACTILE_CORE_API void set_logger(Logger* logger) noexcept;

/** Returns the logger instance used by logging macros. */
[[nodiscard]] TACTILE_CORE_API auto get_logger() noexcept -> Logger*;

}  // namespace tactile
