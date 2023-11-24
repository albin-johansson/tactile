// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/core.h>

#include "tactile/core/api.hpp"
#include "tactile/core/container/smart_ptr.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/debug/log/log_level.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

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

/** \brief A simple logger implementation. */
class TACTILE_CORE_API Logger final {
 public:
  Logger() = default;
  ~Logger() noexcept = default;

  TACTILE_DELETE_COPY(Logger);
  TACTILE_DEFAULT_MOVE(Logger);

  /**
   * \brief Logs an arbitrary message.
   *
   * \param level      the severity of the message.
   * \param fmt_string the format string.
   * \param args       the format arguments.
   */
  template <typename... Args>
  void log(const LogLevel level, const StringView fmt_string, Args&&... args) noexcept
  {
    _log(level, fmt_string, fmt::make_format_args(args...));
  }

  /**
   * \brief Sets the minimum severity of messages that get logged.
   *
   * \param level the minimum log level.
   */
  void set_min_level(LogLevel level) noexcept;

  /**
   * \brief Sets the minimum severity of messages that will initiate flushes.
   *
   * \param level the flush log level.
   */
  void flush_on(LogLevel level) noexcept;

  /**
   * \brief Associates a sink implementation with the logger.
   *
   * \param sink a logger sink implementation.
   */
  void add_sink(Managed<ILoggerSink> sink);

  /**
   * \brief Sets a reference time point to use as a relative baseline for timestamps.
   *
   * \param instant the new reference instant; or nothing to use the default.
   */
  void set_reference_instant(Maybe<SteadyClockInstant> instant);

  /**
   * \brief Indicates whether a message with a specific severity will be logged.
   *
   * \param level the log level to consider.
   *
   * \return true if the message would be logged; false otherwise.
   */
  [[nodiscard]]
  auto would_log(LogLevel level) const noexcept -> bool;

  /**
   * \brief Indicates whether a message with a specific severity will trigger a flush.
   *
   * \param level the log level to consider.
   *
   * \return true if the message would cause a flush; false otherwise.
   */
  [[nodiscard]]
  auto would_flush(LogLevel level) const noexcept -> bool;

  /**
   * \brief Returns a three-letter acronym for the specified log level.
   *
   * \param level the log level to consider.
   *
   * \return a log level acronym.
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
 * \brief Sets the logger instance that is used by the logging macros.
 *
 * \param logger the new default logger.
 *
 * \see TACTILE_LOG_TRACE
 * \see TACTILE_LOG_DEBUG
 * \see TACTILE_LOG_INFO
 * \see TACTILE_LOG_WARN
 * \see TACTILE_LOG_ERROR
 * \see TACTILE_LOG_FATAL
 */
TACTILE_CORE_API void set_default_logger(Logger* logger) noexcept;

/**
 * \brief Returns the logger instance used by the logging macros.
 *
 * \return the current default logger.
 */
[[nodiscard]]
TACTILE_CORE_API auto get_default_logger() noexcept -> Logger*;

}  // namespace tactile
