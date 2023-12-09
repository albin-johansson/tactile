// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/log/logger.hpp"

#include <cstdio>     // fprintf, stderr
#include <exception>  // exception
#include <iterator>   // back_inserter

#include <fmt/chrono.h>

#include "tactile/foundation/log/logger_sink.hpp"

namespace tactile {
namespace {

constinit Logger* gDefaultLogger = nullptr;

}  // namespace

void Logger::_log(const LogLevel level,
                  const StringView fmt_string,
                  const fmt::format_args args) noexcept
{
  try {
    if (!mSinks.empty() && would_log(level)) {
      const auto log_instant = SteadyClock::now();
      const auto text = fmt::vformat(fmt_string, args);

      const auto level_acronym = get_acronym(level);
      const auto elapsed_time = _to_elapsed_time(log_instant);

      fmt::memory_buffer prefix_buffer;
      fmt::format_to(std::back_inserter(prefix_buffer),
                     "[{} {:.>12%Q}]",
                     level_acronym,
                     elapsed_time);

      const LogMessage msg {
        .level = level,
        .prefix = StringView {prefix_buffer.data(), prefix_buffer.size()},
        .text = text,
        .instant = log_instant,
      };

      const auto will_flush = would_flush(level);

      for (const Managed<ILoggerSink>& sink : mSinks) {
        sink->log(msg);

        if (will_flush) {
          sink->flush();
        }
      }
    }
  }
  catch (const std::exception& ex) {
    std::fprintf(stderr, "### LOGGER ERROR: %s ###\n", ex.what());
  }
  catch (...) {
    std::fprintf(stderr, "### UNKNOWN LOGGER ERROR ###\n");
  }
}

void Logger::set_min_level(const LogLevel level) noexcept
{
  mMinLevel = level;
}

void Logger::flush_on(const LogLevel level) noexcept
{
  mFlushLevel = level;
}

void Logger::add_sink(Managed<ILoggerSink> sink)
{
  if (sink) {
    mSinks.push_back(std::move(sink));
  }
}

void Logger::set_reference_instant(const Maybe<SteadyClockInstant> instant)
{
  mReferenceInstant = instant;
}

auto Logger::would_log(const LogLevel level) const noexcept -> bool
{
  return level >= mMinLevel;
}

auto Logger::would_flush(const LogLevel level) const noexcept -> bool
{
  return level >= mFlushLevel;
}

auto Logger::get_acronym(const LogLevel level) noexcept -> StringView
{
  switch (level) {
    case LogLevel::kTrace: return "TRC";
    case LogLevel::kDebug: return "DBG";
    case LogLevel::kInfo: return "INF";
    case LogLevel::kWarn: return "WRN";
    case LogLevel::kError: return "ERR";
    case LogLevel::kFatal: return "FTL";
  }

  return "";
}

auto Logger::_to_elapsed_time(const SteadyClockInstant instant) const -> Microseconds
{
  if (mReferenceInstant.has_value()) {
    return duration_cast<Microseconds>(instant - *mReferenceInstant);
  }

  return duration_cast<Microseconds>(instant.time_since_epoch());
}

void set_default_logger(Logger* logger) noexcept
{
  gDefaultLogger = logger;
}

auto get_default_logger() noexcept -> Logger*
{
  return gDefaultLogger;
}

}  // namespace tactile
