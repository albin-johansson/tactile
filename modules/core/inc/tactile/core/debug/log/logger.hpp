// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <fmt/core.h>

#include "tactile/core/api.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/container/vector.hpp"
#include "tactile/core/debug/log/log_level.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

#define TACTILE_LOG(Level, FmtString, ...)                                   \
  do {                                                                       \
    auto* logger = tactile::get_default_logger();                            \
    if (logger && logger->would_log((Level))) {                              \
      logger->log((Level), (FmtString), fmt::make_format_args(__VA_ARGS__)); \
    }                                                                        \
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

/** \brief A simple logger implementation. */
class Logger final {
 public:
  /**
   * \brief Logs an arbitrary message.
   *
   * \param level      the severity of the message.
   * \param fmt_string the format string.
   * \param args       the format arguments.
   */
  TACTILE_CORE_API void log(LogLevel level,
                            StringView fmt_string,
                            fmt::format_args args) noexcept;

  /**
   * \brief Sets the minimum severity of messages that get logged.
   *
   * \param level the minimum log level.
   */
  TACTILE_CORE_API void set_min_level(LogLevel level) noexcept;

  /**
   * \brief Sets the minimum severity of messages that will initiate flushes.
   *
   * \param level the flush log level.
   */
  TACTILE_CORE_API void flush_on(LogLevel level) noexcept;

  /**
   * \brief Associates a sink implementation with the logger.
   *
   * \param sink a logger sink implementation.
   */
  TACTILE_CORE_API void add_sink(ILoggerSink* sink);

  /**
   * \brief Sets a reference time point to use as a relative baseline for timestamps.
   *
   * \param instant the new reference instant; or nothing to use the default.
   */
  TACTILE_CORE_API void set_reference_instant(Maybe<SteadyClockInstant> instant);

  /**
   * \brief Indicates whether a message with a specific severity will be logged.
   *
   * \param level the log level to consider.
   *
   * \return true if the message would be logged; false otherwise.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto would_log(LogLevel level) const noexcept -> bool;

  /**
   * \brief Indicates whether a message with a specific severity will trigger a flush.
   *
   * \param level the log level to consider.
   *
   * \return true if the message would cause a flush; false otherwise.
   */
  [[nodiscard]]
  TACTILE_CORE_API auto would_flush(LogLevel level) const noexcept -> bool;

  /**
   * \brief Returns a three-letter acronym for the specified log level.
   *
   * \param level the log level to consider.
   *
   * \return a log level acronym.
   */
  [[nodiscard]]
  TACTILE_CORE_API static auto get_acronym(LogLevel level) noexcept -> StringView;

 private:
  LogLevel mMinLevel {LogLevel::kInfo};
  LogLevel mFlushLevel {LogLevel::kError};
  Vector<ILoggerSink*> mSinks;
  Maybe<SteadyClockInstant> mReferenceInstant;

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
