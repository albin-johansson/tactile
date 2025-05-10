// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

export module tactile.log:logger;

export import std;
export import tactile.core;
export import :level;
export import :sink;
import :buffer;

export namespace tactile {

/// A sink based logger.
class Logger final
{
 public:
  using clock_type = SteadyClock;

  Logger() noexcept = default;

  Logger(Logger&&) = delete;

  Logger(const Logger&) = delete;

  ~Logger() noexcept = default;

  auto operator=(Logger&&) -> Logger& = delete;

  auto operator=(const Logger&) -> Logger& = delete;

  /// Logs a formatted message.
  template <typename... Args>
  void log(const LogLevel level,
           const std::format_string<Args...> fmt,
           const Args&... args) noexcept
  {
    if (level >= m_min_level) {
      _log(level, fmt.get(), std::make_format_args(args...));
    }
  }

  /// Disposes all associated sinks.
  void reset();

  /// Adds a sink to the logger.
  void add_sink(Unique<ILogSink> sink);

  /// Sets the minimum level of logged messages.
  void set_min_level(LogLevel level);

  /// Sets the level at which messages will be explicitly flushed.
  void set_flush_level(LogLevel level);

  /// Sets the start time (used to customize timestamps).
  void set_start_time(Option<clock_type::time_point> start_time);

 private:
  LogLevel m_min_level {LogLevel::kInfo};
  LogLevel m_flush_level {LogLevel::kError};
  Option<clock_type::time_point> m_start_time {};
  Vector<Unique<ILogSink>> m_sinks {};
  LogBuffer<1024> m_text_buffer {};
  LogBuffer<32> m_prefix_buffer {};

  void _log(LogLevel level, StringView fmt, std::format_args args) noexcept;
};

/// Returns the global logger instance.
[[nodiscard]]
auto get_logger() noexcept -> Logger&;

}  // namespace tactile
