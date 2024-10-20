// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/log/logger.hpp"

#include <cstdio>     // fprintf, stderr
#include <exception>  // exception
#include <iterator>   // back_inserter

#include "tactile/base/container/buffer.hpp"

namespace tactile::core {
namespace {

constinit Logger* gDefaultLogger = nullptr;  // NOLINT

}  // namespace

void Logger::_log(const LogLevel level,
                  const std::string_view fmt_string,
                  const std::format_args args) noexcept
{
  try {
    if (!mSinks.empty() && would_log(level)) {
      const auto log_instant = std::chrono::steady_clock::now();

      Buffer<char, 512> text_buffer;  // NOLINT uninitialized
      vformat_to_buffer(text_buffer, fmt_string, args);

      const auto level_acronym = get_acronym(level);
      const auto elapsed_time = _to_elapsed_time(log_instant);

      Buffer<char, 64> prefix_buffer;  // NOLINT uninitialized
      format_to_buffer(prefix_buffer, "[{} {:.>12%Q}]", level_acronym, elapsed_time);

      const LogMessage msg {
        .level = level,
        .prefix = prefix_buffer.view(),
        .scope = mScope,
        .text = text_buffer.view(),
        .instant = log_instant,
      };

      const auto will_flush = would_flush(level);

      for (const auto& sink : mSinks) {
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

void Logger::add_sink(std::unique_ptr<ILogSink> sink)
{
  if (sink) {
    mSinks.push_back(std::move(sink));
  }
}

void Logger::set_reference_instant(
    const std::optional<std::chrono::steady_clock::time_point> instant)
{
  mReferenceInstant = instant;
}

void Logger::set_scope(const std::string_view scope) noexcept
{
  mScope = scope;
}

auto Logger::get_scope() const noexcept -> std::string_view
{
  return mScope;
}

auto Logger::would_log(const LogLevel level) const noexcept -> bool
{
  return level >= mMinLevel;
}

auto Logger::would_flush(const LogLevel level) const noexcept -> bool
{
  return level >= mFlushLevel;
}

auto Logger::get_acronym(const LogLevel level) noexcept -> std::string_view
{
  switch (level) {
    case LogLevel::kTrace: return "TRC";
    case LogLevel::kDebug: return "DBG";
    case LogLevel::kInfo:  return "INF";
    case LogLevel::kWarn:  return "WRN";
    case LogLevel::kError: return "ERR";
    case LogLevel::kFatal: return "FTL";
  }

  return "";
}

auto Logger::_to_elapsed_time(const std::chrono::steady_clock::time_point instant) const
    -> std::chrono::microseconds
{
  if (mReferenceInstant.has_value()) {
    return duration_cast<std::chrono::microseconds>(instant - *mReferenceInstant);
  }

  return duration_cast<std::chrono::microseconds>(instant.time_since_epoch());
}

void set_default_logger(Logger* logger) noexcept
{
  gDefaultLogger = logger;
}

auto get_default_logger() noexcept -> Logger*
{
  return gDefaultLogger;
}

}  // namespace tactile::core
