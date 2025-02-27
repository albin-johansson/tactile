// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/basic/macros.hpp"
#include "tactile/core/basic/string.hpp"
#include "tactile/core/log/log_level.hpp"

namespace tactile {

/// Represents a single logged message.
///
/// Note, the included string views must not be stored beyond the scope of log function calls.
/// If a sink wants to keep a history of log messages, it has to copy the string views to
/// ordinary strings.
struct LogMessage final
{
  /// The severity of the message.
  LogLevel level;

  /// A string that usually encodes the severity and timestamp.
  StringView prefix;

  /// The formatted log message.
  StringView text;
};

/// Interface for logger sinks.
///
/// A logger may feature several associated sinks which are called whenever a message is
/// logged. This allows the application to control exactly what happens with logged messages,
/// e.g., whether to print them to a console or store them in a file.
class ILogSink
{
 public:
  TACTILE_INTERFACE_CLASS(ILogSink);

  /// Logs an incoming message.
  virtual void log(const LogMessage& msg) = 0;

  /// Flushes any pending messages, most likely via an associated I/O stream.
  virtual void flush() = 0;
};

}  // namespace tactile
