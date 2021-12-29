#pragma once

#include <string>       // string
#include <string_view>  // string_view

#include <fmt/format.h>
#include <fmt/ostream.h>  // included so that formatting types like std::filesystem::path works
#include <tactile_def.hpp>

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

void ClearLogHistory();

void SetLogLevel(LogLevel level);

[[nodiscard]] auto GetLoggedString(usize index) -> const std::string&;

[[nodiscard]] auto GetLogSize() -> usize;

[[nodiscard]] auto GetLogLevel() -> LogLevel;

}  // namespace Tactile
