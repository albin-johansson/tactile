// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module tactile.log;

import std;

namespace tactile {
namespace {

[[nodiscard]]
auto _to_prefix(const LogLevel level) noexcept -> StringView
{
  switch (level) {
    case LogLevel::kTrace: return "TRC";
    case LogLevel::kDebug: return "DBG";
    case LogLevel::kInfo:  return "INF";
    case LogLevel::kWarn:  return "WRN";
    case LogLevel::kError: return "ERR";
    default:               return "???";
  }
}

}  // namespace

void Logger::_log(const LogLevel level,
                  const StringView fmt,
                  const std::format_args args) noexcept
{
  try {
    if (m_sinks.empty()) {
      return;
    }

    const auto log_instant = clock_type::now();
    const auto elapsed_time = duration_cast<Microseconds>(
        log_instant - m_start_time.value_or(clock_type::time_point {}));

    m_text_buffer.clear();
    m_prefix_buffer.clear();

    std::vformat_to(std::back_inserter(m_text_buffer), fmt, args);
    std::format_to(std::back_inserter(m_prefix_buffer),
                   "[{} {:.>12%Q}]",
                   _to_prefix(level),
                   elapsed_time);

    const LogMessage message {
      .level = level,
      .prefix = m_prefix_buffer.view(),
      .text = m_text_buffer.view(),
    };

    const auto should_flush = level >= m_flush_level;
    for (const auto& sink : m_sinks) {
      sink->log(message);

      if (should_flush) {
        sink->flush();
      }
    }
  }
  catch (const std::exception& error) {
    std::printf("LOGGER ERROR: %s\n", error.what());
  }
  catch (...) {
    std::printf("UNKNOWN LOGGER ERROR\n");
  }
}

void Logger::reset()
{
  m_sinks.clear();
}

void Logger::add_sink(Unique<ILogSink> sink)
{
  m_sinks.push_back(std::move(sink));
}

void Logger::set_min_level(const LogLevel level)
{
  m_min_level = level;
}

void Logger::set_flush_level(const LogLevel level)
{
  m_flush_level = level;
}

void Logger::set_start_time(const Option<clock_type::time_point> start_time)
{
  m_start_time = start_time;
}

auto get_logger() noexcept -> Logger&
{
  static Logger logger {};
  return logger;
}

}  // namespace tactile
