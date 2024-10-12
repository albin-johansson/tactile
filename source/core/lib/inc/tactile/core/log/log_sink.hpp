// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <chrono>       // steady_clock
#include <string_view>  // string_view

#include "tactile/base/log/log_level.hpp"
#include "tactile/base/prelude.hpp"

namespace tactile::core {

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
  LogLevel level;           ///< The severity of the message.
  std::string_view prefix;  ///< A string that encodes the severity and timestamp.
  std::string_view scope;   ///< Scope identifier, might be empty.
  std::string_view text;    ///< The formatted log message.
  std::chrono::steady_clock::time_point instant;  ///< The instant that the message was logged.
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

}  // namespace tactile::core
