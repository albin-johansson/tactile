// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/container/string.hpp"
#include "tactile/core/debug/log/log_level.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"

namespace tactile {

/**
 * \brief Represents a single logged message.
 *
 * \note The included string views must not be stored beyond the scope of log function
 *       calls. If a sink wants to keep a history of log messages, it has to copy the
 *       string views to ordinary strings.
 */
struct LogMessage final {
  LogLevel level;              ///< The severity of the message.
  StringView prefix;           ///< A string that encodes the severity and timestamp.
  StringView text;             ///< The formatted log message.
  SteadyClockInstant instant;  ///< The instant that the message was logged.
};

/**
 * \interface ILoggerSink
 *
 * \brief Interface for logger "sink" implementations.
 *
 * \details A logger may feature several associated sinks which are called
 *          whenever a message is logged. This allows for the application to
 *          control exactly what happens with logged messages, e.g., whether to
 *          print them to a console or store them in a file.
 */
class ILoggerSink {
 public:
  TACTILE_INTERFACE_CLASS(ILoggerSink);

  /**
   * \brief Logs an incoming message.
   *
   * \param msg the message information.
   */
  virtual void log(const LogMessage& msg) = 0;

  /** \brief Flushes any pending state, most likely an associated I/O stream. */
  virtual void flush() = 0;
};

}  // namespace tactile
