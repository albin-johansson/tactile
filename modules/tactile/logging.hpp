#pragma once

#include <string>       // string
#include <string_view>  // string_view

#include <fmt/format.h>
#include <fmt/ostream.h>  // included so that formatting types like std::filesystem::path works
#include <tactile_def.hpp>

namespace Tactile {
namespace LoggerImpl {

void LogDebugV(std::string_view fmt, fmt::format_args args);
void LogInfoV(std::string_view fmt, fmt::format_args args);
void LogWarningV(std::string_view fmt, fmt::format_args args);
void LogErrorV(std::string_view fmt, fmt::format_args args);

}  // namespace LoggerImpl

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

[[nodiscard]] auto GetLoggedString(usize index) -> const std::string&;

[[nodiscard]] auto GetLogSize() -> usize;

}  // namespace Tactile
