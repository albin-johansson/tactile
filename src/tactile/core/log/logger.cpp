// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/core/log/logger.hpp"

#include <cstdio>
#include <exception>
#include <iterator>
#include <print>
#include <stdexcept>
#include <utility>

namespace tactile {
namespace {

/// This is the global logger instance.
inline constinit Logger* gLogger = nullptr;  // NOLINT(*-avoid-non-const-global-variables)

auto to_prefix_label(const LogCategory category) noexcept -> StringView
{
  switch (category) {
    case LogCategory::kVerbose: return "VER";
    case LogCategory::kDebug:   return "DBG";
    case LogCategory::kInfo:    return "INF";
    case LogCategory::kWarn:    return "WRN";
    case LogCategory::kError:   return "ERR";
    default:                    return "???";
  }
}

}  // namespace

void Logger::add_sink(Unique<ILogSink> sink)
{
  if (sink == nullptr) {
    throw std::invalid_argument {"[Logger::add_sink]: null sink"};
  }
  m_sinks.push_back(std::move(sink));
}

void Logger::set_ref_instant(const clock_type::time_point instant) noexcept
{
  m_ref_instant = instant;
}

void Logger::set_min_category(const LogCategory category) noexcept
{
  m_min_category = category;
}

void Logger::set_flush_category(const LogCategory category) noexcept
{
  m_flush_category = category;
}

auto Logger::would_log(const LogCategory category) const noexcept -> bool
{
  return category >= m_min_category;
}

void Logger::log_impl(const LogCategory category,
                      const StringView fmt,
                      const std::format_args& args) noexcept
{
  try {
    if (m_sinks.empty()) {
      return;
    }

    const auto now = clock_type::now();
    const auto elapsed_time =
        std::chrono::duration_cast<std::chrono::microseconds>(now - m_ref_instant);

    const auto category_label = to_prefix_label(category);

    m_prefix_buffer.clear();
    (void) std::format_to(std::back_inserter(m_prefix_buffer),
                          "[{}]({:%Q}us):",
                          category_label,
                          elapsed_time);

    m_message_buffer.clear();
    (void) std::vformat_to(std::back_inserter(m_message_buffer), fmt, args);

    const LogEntry entry {
      .category = category,
      .prefix = m_prefix_buffer.view(),
      .message = m_message_buffer.view(),
    };

    for (const auto& sink : m_sinks) {
      sink->log(entry);

      if (category >= m_flush_category) {
        sink->flush();
      }
    }
  }
  catch (const std::exception& error) {
    (void) std::fprintf(stderr, "[Logger]: caught exception: %s\n", error.what());
  }
  catch (...) {
    (void) std::fprintf(stderr, "[Logger]: caught unknown exception\n");
  }
}

void set_logger(Logger* const logger) noexcept
{
  gLogger = logger;
}

auto get_logger() noexcept -> Logger*
{
  return gLogger;
}

}  // namespace tactile
