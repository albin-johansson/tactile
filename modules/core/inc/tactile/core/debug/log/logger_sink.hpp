// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/debug/log/log_level.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/chrono.hpp"
#include "tactile/core/type/string.hpp"

namespace tactile {

/**
 * Represents a single logged message.
 *
 * \note The included string views must not be stored beyond the scope of
 *       log function calls. If a sink wants to keep a history of log messages,
 *       it has to copy the string views into ordinary strings.
 *
 * \var   level the severity of the message.
 * \var  prefix a string that encodes the severity and timestamp. Each sink is
 *              free to decide whether to include this in their output.
 * \var    text the formatted log message.
 * \var instant the instant that the message was logged.
 */
struct TACTILE_CORE_API LogMessage final {
  LogLevel level;
  StringView prefix;
  StringView text;
  SteadyClockInstant instant;
};

/**
 * Interface for logger "sink" implementations.
 *
 * \details A logger may feature several associated sinks which are called
 *          whenever a message is logged. This allows for the application to
 *          control exactly what happens with logged messages, e.g., whether to
 *          print them to a console or store them in a file.
 */
class TACTILE_CORE_API ILoggerSink {
 public:
  TACTILE_INTERFACE_CLASS(ILoggerSink);

  /** Logs an incoming message. */
  virtual void log(const LogMessage& msg) = 0;

  /** Flushes any pending state, most likely an associated I/O stream. */
  virtual void flush() = 0;
};

}  // namespace tactile
