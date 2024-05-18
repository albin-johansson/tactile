// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <format>  // format_string, make_format_args, format_args

#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/log/log_level.hpp"
#include "tactile/core/log/log_sink.hpp"

#define TACTILE_LOG(Level, FmtString, ...)                                  \
  {                                                                         \
    auto* tactile_logger = tactile::get_default_logger();                   \
    if (tactile_logger && tactile_logger->would_log((Level))) {             \
      tactile_logger->log((Level), (FmtString) __VA_OPT__(, ) __VA_ARGS__); \
    }                                                                       \
  }                                                                         \
  static_assert(true)

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

class ILogSink;

/**
 * A simple sink-based logger implementation.
 */
class Logger final
{
 public:
  Logger() = default;

  ~Logger() noexcept = default;

  TACTILE_DELETE_COPY(Logger);
  TACTILE_DEFAULT_MOVE(Logger);

  /**
   * Logs an arbitrary message.
   *
   * \tparam Args The format argument types.
   *
   * \param level The severity of the message.
   * \param fmt   The format string.
   * \param args  The format arguments.
   */
  template <typename... Args>
  void log(const LogLevel level,
           const std::format_string<Args...> fmt,
           const Args&... args) noexcept
  {
    _log(level, fmt.get(), std::make_format_args(args...));
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
  void add_sink(Unique<ILogSink> sink);

  /**
   * Sets a reference time point to use as a relative baseline for timestamps.
   *
   * \param instant The new reference instant; or nothing to use the default.
   */
  void set_reference_instant(Maybe<SteadyClockInstant> instant);

  /**
   * Sets a custom scope identifier to use.
   *
   * \param scope An arbitrary scope identifier.
   */
  void set_scope(StringView scope) noexcept;

  /**
   * Returns the current scope identifier.
   *
   * \return
   * An arbitrary string.
   */
  [[nodiscard]]
  auto get_scope() const noexcept -> StringView;

  /**
   * Indicates whether a message with a specific severity will be logged.
   *
   * \param level The log level to consider.
   *
   * \return
   * True if the message would be logged; false otherwise.
   */
  [[nodiscard]]
  auto would_log(LogLevel level) const noexcept -> bool;

  /**
   * Indicates whether a message with a specific severity will trigger a flush.
   *
   * \param level The log level to consider.
   *
   * \return
   * True if the message would cause a flush; false otherwise.
   */
  [[nodiscard]]
  auto would_flush(LogLevel level) const noexcept -> bool;

  /**
   * Returns a three-letter acronym for the specified log level.
   *
   * \param level The log level to consider.
   *
   * \return
   * A log level acronym.
   */
  [[nodiscard]]
  static auto get_acronym(LogLevel level) noexcept -> StringView;

 private:
  LogLevel mMinLevel {LogLevel::kInfo};
  LogLevel mFlushLevel {LogLevel::kError};
  Vector<Unique<ILogSink>> mSinks {};
  Maybe<SteadyClockInstant> mReferenceInstant {};
  StringView mScope {};

  void _log(LogLevel level,
            StringView fmt_string,
            std::format_args args) noexcept;

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
void set_default_logger(Logger* logger) noexcept;

/**
 * Returns the logger instance used by the logging macros.
 *
 * \return
 * The current default logger.
 */
[[nodiscard]]
auto get_default_logger() noexcept -> Logger*;

}  // namespace tactile
