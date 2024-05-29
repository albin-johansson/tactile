// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/string.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/chrono.hpp"
#include "tactile/core/log/log_level.hpp"

namespace tactile {

/**
 * Represents a single logged message.
 *
 * \note
 * The included string views must not be stored beyond the scope of log function
 * calls. If a sink wants to keep a history of log messages, it has to copy the
 * string views to ordinary strings.
 */
struct LogMessage final
{
  LogLevel level;     ///< The severity of the message.
  StringView prefix;  ///< A string that encodes the severity and timestamp.
  StringView scope;   ///< Scope identifier, might be empty.
  StringView text;    ///< The formatted log message.
  SteadyClockInstant instant;  ///< The instant that the message was logged.
};

/**
 * Interface for logger "sink" implementations.
 *
 * \details
 * A logger may feature several associated sinks which are called whenever a
 * message is logged. This allows for the application to control exactly what
 * happens with logged messages, e.g., whether to print them to a console or
 * store them in a file.
 */
class ILogSink
{
 public:
  TACTILE_INTERFACE_CLASS(ILogSink);

  /**
   * Logs an incoming message.
   *
   * \param msg The message information.
   */
  virtual void log(const LogMessage& msg) = 0;

  /**
   * Flushes any pending state, most likely an associated I/O stream.
   */
  virtual void flush() = 0;
};

}  // namespace tactile
