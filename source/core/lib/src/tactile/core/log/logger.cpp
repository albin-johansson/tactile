// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/log/logger.hpp"

#include <cstdio>     // fprintf, stderr
#include <exception>  // exception
#include <iterator>   // back_inserter
#include <utility>    // move

#include <boost/container/static_vector.hpp>
#include <fmt/chrono.h>

#include "tactile/core/basic/option.hpp"
#include "tactile/core/basic/vector.hpp"

namespace tactile {
namespace {

// NOLINTNEXTLINE(*-non-const-global-variables)
inline constinit Logger* gLogger {};

[[nodiscard]]
auto _to_prefix(const LogLevel level) noexcept -> std::string_view
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

struct Logger::Data final
{
  LogLevel log_level {LogLevel::kInfo};
  LogLevel flush_level {LogLevel::kError};
  Vector<Unique<ILogSink>> sinks {};
  Option<clock_type::time_point> ref_instant {};
  boost::container::static_vector<char, 1024> text_buffer {};
  boost::container::static_vector<char, 20> prefix_buffer {};
};

Logger::Logger()
  : m_data {make_unique<Data>()}
{}

Logger::Logger(Logger&&) noexcept = default;

Logger::~Logger() noexcept = default;

auto Logger::operator=(Logger&&) noexcept -> Logger& = default;

// ReSharper disable once CppMemberFunctionMayBeConst
void Logger::add_sink(Unique<ILogSink> sink)
{
  m_data->sinks.push_back(std::move(sink));
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Logger::set_reference_instant(const clock_type::time_point instant)
{
  m_data->ref_instant = instant;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Logger::set_log_level(const LogLevel level)
{
  m_data->log_level = level;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void Logger::set_flush_level(const LogLevel level)
{
  m_data->flush_level = level;
}

auto Logger::would_log(const LogLevel level) const noexcept -> bool
{
  return level >= m_data->log_level;
}

auto Logger::would_flush(const LogLevel level) const noexcept -> bool
{
  return level >= m_data->flush_level;
}

void Logger::_log(const LogLevel level,
                  const StringView fmt,
                  const fmt::format_args& args) noexcept
{
  try {
    auto& data = *m_data;
    if (data.sinks.empty()) {
      return;
    }

    const auto log_instant = clock_type::now();
    const auto elapsed_time = duration_cast<Microseconds>(
        log_instant - data.ref_instant.value_or(clock_type::time_point {}));

    data.prefix_buffer.clear();
    fmt::format_to_n(std::back_inserter(data.prefix_buffer),
                     data.prefix_buffer.capacity(),
                     "[{} {:.>12%Q}]",
                     _to_prefix(level),
                     elapsed_time);

    data.text_buffer.clear();
    fmt::vformat_to_n(std::back_inserter(data.text_buffer),
                      data.text_buffer.capacity(),
                      fmt,
                      args);

    const LogMessage message {
      .level = level,
      .prefix = StringView {data.prefix_buffer.data(), data.prefix_buffer.size()},
      .text = StringView {data.text_buffer.data(), data.text_buffer.size()},
    };

    const auto do_flush = would_flush(level);

    for (const auto& sink : data.sinks) {
      sink->log(message);

      if (do_flush) {
        sink->flush();
      }
    }
  }
  catch (const std::exception& exception) {
    // NOLINTNEXTLINE(*-use-std-print)
    std::fprintf(stderr, "Logger error: %s", exception.what());
  }
  catch (...) {
    // NOLINTNEXTLINE(*-use-std-print)
    std::fprintf(stderr, "Logger error");
  }
}

void set_logger(Logger* logger) noexcept
{
  gLogger = logger;
}

auto get_logger() noexcept -> Logger*
{
  return gLogger;
}

}  // namespace tactile
