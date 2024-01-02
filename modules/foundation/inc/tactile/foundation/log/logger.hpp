// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/core.h>

#include "tactile/foundation/api.hpp"
#include "tactile/foundation/container/smart_ptr.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/log/log_level.hpp"
#include "tactile/foundation/misc/chrono.hpp"
#include "tactile/foundation/prelude.hpp"

#define TACTILE_LOG(Level, FmtString, ...)                                  \
  do {                                                                      \
    auto* tactile_logger = tactile::get_default_logger();                   \
    if (tactile_logger && tactile_logger->would_log((Level))) {             \
      tactile_logger->log((Level), (FmtString) __VA_OPT__(, ) __VA_ARGS__); \
    }                                                                       \
  } while (false)

#define TACTILE_LOG_TRACE(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kTrace, FmtString, __VA_ARGS__)

#define TACTILE_LOG_DEBUG(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kDebug, FmtString, __VA_ARGS__)

#define TACTILE_LOG_INFO(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kInfo, FmtString, __VA_ARGS__)

#define TACTILE_LOG_WARN(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kWarn, FmtString, __VA_ARGS__)

#define TACTILE_LOG_ERROR(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kError, FmtString, __VA_ARGS__)

#define TACTILE_LOG_FATAL(FmtString, ...) \
  TACTILE_LOG(tactile::LogLevel::kFatal, FmtString, __VA_ARGS__)

namespace tactile {

class ILoggerSink;

/**
 * A simple sink-based logger implementation.
 */
class TACTILE_FOUNDATION_API Logger final {
 public:
  Logger() = default;
  ~Logger() noexcept = default;

  TACTILE_DELETE_COPY(Logger);
  TACTILE_DEFAULT_MOVE(Logger);

  /**
   * Logs an arbitrary message.
   *
   * \param level      The severity of the message.
   * \param fmt_string The format string.
   * \param args       The format arguments.
   */
  template <typename... Args>
  void log(const LogLevel level, const StringView fmt_string, Args&&... args) noexcept
  {
    _log(level, fmt_string, fmt::make_format_args(args...));
  }

  /**
   * Sets the minimum severity of messages that get logged.
   *
   * \param level The minimum log level.
   */
  void set_min_level(LogLevel level) noexcept;

  /**
   * Sets the minimum severity of messages that will initiate flushes.
   *
   * \param level The flush log level.
   */
  void flush_on(LogLevel level) noexcept;

  /**
   * Associates a sink implementation with the logger.
   *
   * \param sink A logger sink implementation.
   */
  void add_sink(Managed<ILoggerSink> sink);

  /**
   * Sets a reference time point to use as a relative baseline for timestamps.
   *
   * \param instant The new reference instant; or nothing to use the default.
   */
  void set_reference_instant(Maybe<SteadyClockInstant> instant);

  /**
   * Indicates whether a message with a specific severity will be logged.
   *
   * \param level The log level to consider.
   *
   * \return
   *    True if the message would be logged; false otherwise.
   */
  [[nodiscard]]
  auto would_log(LogLevel level) const noexcept -> bool;

  /**
   * Indicates whether a message with a specific severity will trigger a flush.
   *
   * \param level The log level to consider.
   *
   * \return
   *    True if the message would cause a flush; false otherwise.
   */
  [[nodiscard]]
  auto would_flush(LogLevel level) const noexcept -> bool;

  /**
   * Returns a three-letter acronym for the specified log level.
   *
   * \param level The log level to consider.
   *
   * \return
   *    A log level acronym.
   */
  [[nodiscard]]
  static auto get_acronym(LogLevel level) noexcept -> StringView;

 private:
  LogLevel mMinLevel {LogLevel::kInfo};
  LogLevel mFlushLevel {LogLevel::kError};
  Vector<Managed<ILoggerSink>> mSinks;
  Maybe<SteadyClockInstant> mReferenceInstant;

  void _log(LogLevel level, StringView fmt_string, fmt::format_args args) noexcept;

  [[nodiscard]]
  auto _to_elapsed_time(SteadyClockInstant instant) const -> Microseconds;
};

/**
 * Sets the logger instance that is used by the logging macros.
 *
 * \param logger The new default logger.
 *
 * \see `TACTILE_LOG_TRACE`
 * \see `TACTILE_LOG_DEBUG`
 * \see `TACTILE_LOG_INFO`
 * \see `TACTILE_LOG_WARN`
 * \see `TACTILE_LOG_ERROR`
 * \see `TACTILE_LOG_FATAL`
 */
TACTILE_FOUNDATION_API void set_default_logger(Logger* logger) noexcept;

/**
 * Returns the logger instance used by the logging macros.
 *
 * \return
 *    The current default logger.
 */
[[nodiscard]]
TACTILE_FOUNDATION_API auto get_default_logger() noexcept -> Logger*;

}  // namespace tactile
