// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

export module tactile.log:sink;

export import tactile.common;
export import :level;

export namespace tactile {

/// A view of a logged message.
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
/// A logger may feature several associated sinks which are called whenever a message
/// is logged. This allows the application to control exactly what happens with
/// logged messages, e.g., whether to print them to a console or store them in a
/// file.
class ILogSink
{
 protected:
  ILogSink() = default;

  ILogSink(ILogSink&&) noexcept = default;

  ILogSink(const ILogSink&) = default;

  auto operator=(ILogSink&&) noexcept -> ILogSink& = default;

  auto operator=(const ILogSink&) -> ILogSink& = default;

 public:
  virtual ~ILogSink() noexcept = default;

  /// Logs an incoming message.
  virtual void log(const LogMessage& msg) = 0;

  /// Flushes any pending messages, most likely via an associated I/O stream.
  virtual void flush() = 0;
};

}  // namespace tactile
