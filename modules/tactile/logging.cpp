#include "logging.hpp"

#include <ctime>  // time
#include <deque>  // deque

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>

#include "build.hpp"

namespace Tactile {
namespace {

inline std::deque<std::string> gHistory;
inline LogLevel gLogLevel{LogLevel::Info};

void Log(const fmt::color color,
         const std::string_view priority,
         const std::string_view fmt,
         const fmt::format_args args)
{
  const auto time = std::time(nullptr);
  const auto msg = fmt::vformat(fmt, args);
  const auto full = fmt::format("{:%H:%M:%S} > {}\n", fmt::localtime(time), msg);

  gHistory.push_back(full);

  if constexpr (IsDebugBuild()) {
    if constexpr (IsPlatformWindows()) {
      fmt::print("{:>9} {}", priority, full);
    }
    else {
      fmt::print(fmt::fg(color), "{:>9} {}", priority, full);
    }
  }
}

}  // namespace

namespace LoggerImpl {

[[nodiscard]] constexpr auto IsEnabled(const LogLevel level) -> bool
{
  switch (level) {
    case LogLevel::Verbose:
      return gLogLevel == LogLevel::Verbose;

    case LogLevel::Debug:
      return gLogLevel == LogLevel::Verbose || gLogLevel == LogLevel::Debug;

    case LogLevel::Info:
      return gLogLevel == LogLevel::Verbose || gLogLevel == LogLevel::Debug ||
             gLogLevel == LogLevel::Info;

    case LogLevel::Warning:
      return gLogLevel == LogLevel::Verbose || gLogLevel == LogLevel::Debug ||
             gLogLevel == LogLevel::Info || gLogLevel == LogLevel::Warning;

    case LogLevel::Error:
      return true; /* Errors are always logged */

    default:
      return false;
  }
}

void LogVerboseV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Verbose)) {
    Log(fmt::color::violet, "[VERBOSE]", fmt, args);
  }
}

void LogDebugV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Debug)) {
    Log(fmt::color::aquamarine, "[DEBUG]", fmt, args);
  }
}

void LogInfoV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Info)) {
    Log(fmt::color::white, "[INFO]", fmt, args);
  }
}

void LogWarningV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Warning)) {
    Log(fmt::color::yellow, "[WARNING]", fmt, args);
  }
}

void LogErrorV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Error)) {
    Log(fmt::color::orange_red, "[ERROR]", fmt, args);
  }
}

}  // namespace LoggerImpl

void ClearLogHistory()
{
  gHistory.clear();
}

void SetLogLevel(const LogLevel level)
{
  gLogLevel = level;
}

auto GetLoggedString(const usize index) -> const std::string&
{
  return gHistory.at(index);
}

auto GetLogSize() -> usize
{
  return gHistory.size();
}

auto GetLogLevel() -> LogLevel
{
  return gLogLevel;
}

}  // namespace Tactile
