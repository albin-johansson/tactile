#pragma once

#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // pair

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>  // included so that formatting types like std::filesystem::path works
#include <tactile_def.hpp>

#include "build.hpp"

namespace Tactile {
namespace LoggerImpl {

void LogVerboseV(std::string_view fmt, fmt::format_args args);
void LogDebugV(std::string_view fmt, fmt::format_args args);
void LogInfoV(std::string_view fmt, fmt::format_args args);
void LogWarningV(std::string_view fmt, fmt::format_args args);
void LogErrorV(std::string_view fmt, fmt::format_args args);

}  // namespace LoggerImpl

enum class LogLevel {
  Verbose,  ///< Logs everything.
  Debug,    ///< Logs stuff that a developer might be interested in.
  Info,     ///< Default log level, emit informational stuff for ordinary users.
  Warning,  ///< Only emit warnings and errors.
  Error     ///< Only emit errors.
};

template <typename... Args>
void Print([[maybe_unused]] const fmt::color color,
           const std::string_view fmt,
           const Args&... args)
{
  if constexpr (IsPlatformWindows()) {
    fmt::print(fmt, args...);
  }
  else {
    fmt::print(fmt::fg(color), fmt, args...);
  }
}

template <typename... Args>
void LogVerbose(const std::string_view fmt, const Args&... args)
{
  LoggerImpl::LogVerboseV(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void LogDebug(const std::string_view fmt, const Args&... args)
{
  LoggerImpl::LogDebugV(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void LogInfo(const std::string_view fmt, const Args&... args)
{
  LoggerImpl::LogInfoV(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void LogWarning(const std::string_view fmt, const Args&... args)
{
  LoggerImpl::LogWarningV(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void LogError(const std::string_view fmt, const Args&... args)
{
  LoggerImpl::LogErrorV(fmt, fmt::make_format_args(args...));
}

/// \brief Clears the entire log history.
void ClearLogHistory();

void SetLogLevel(LogLevel level);

/**
 * \brief Returns the log entry at a specific index amongst entries that satisfy a filter.
 *
 * \param filter the log level filter to apply before querying log entries.
 * \param index the index of the desired log entry among those that satisfy the filter.
 *
 * \return a pair of the found log level and logged message.
 *
 * \throws TactileError if no log entry was found.
 *
 * \see GetLogSize(LogLevel)
 */
[[nodiscard]] auto GetFilteredLogEntry(LogLevel filter, usize index)
    -> std::pair<LogLevel, const std::string&>;

/**
 * \brief Returns the amount of log entries that satisfy a filter.
 *
 * \param filter the filter that will be used.
 *
 * \return the amount of log entries that weren't filtered out.
 */
[[nodiscard]] auto GetLogSize(LogLevel filter) -> usize;

/**
 * \brief Indicates whether a message using a specific log level satisfies a filter.
 *
 * \param filter the filter level to use.
 * \param level the log level to check.
 *
 * \return `true` if the log level is enabled according to the filter; `false` otherwise.
 */
[[nodiscard]] auto IsEnabled(LogLevel filter, LogLevel level) -> bool;

[[nodiscard]] auto IsEnabled(LogLevel level) -> bool;

}  // namespace Tactile
