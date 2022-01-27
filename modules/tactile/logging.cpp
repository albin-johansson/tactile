#include "logging.hpp"

#include <ctime>  // time
#include <deque>  // deque

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <tactile_stdlib.hpp>

#include "throw.hpp"

namespace tactile {
namespace {

struct LoggedString final
{
  LogLevel level{};
  std::string str;
};

inline std::deque<LoggedString> gHistory;
inline LogLevel gLogLevel{LogLevel::Info};

void Log(const fmt::color color,
         const LogLevel level,
         const std::string_view priority,
         const std::string_view fmt,
         const fmt::format_args args)
{
  const auto time = fmt::localtime(std::time(nullptr));
  const auto msg = fmt::vformat(fmt, args);
  const auto full = fmt::vformat("{:%H:%M:%S} > {}\n", fmt::make_format_args(time, msg));

  gHistory.push_back(LoggedString{level, full});

  if constexpr (is_debug_build) {
    Print(color, "{:>9} {}", priority, full);
  }
}

}  // namespace

namespace LoggerImpl {

void LogVerboseV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Verbose)) {
    Log(fmt::color::violet, LogLevel::Verbose, "[VERBOSE]", fmt, args);
  }
}

void LogDebugV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Debug)) {
    Log(fmt::color::aquamarine, LogLevel::Debug, "[DEBUG]", fmt, args);
  }
}

void LogInfoV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Info)) {
    Log(fmt::color::white, LogLevel::Info, "[INFO]", fmt, args);
  }
}

void LogWarningV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Warning)) {
    Log(fmt::color::yellow, LogLevel::Warning, "[WARNING]", fmt, args);
  }
}

void LogErrorV(const std::string_view fmt, const fmt::format_args args)
{
  if (IsEnabled(LogLevel::Error)) {
    Log(fmt::color::orange_red, LogLevel::Error, "[ERROR]", fmt, args);
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

auto GetFilteredLogEntry(const LogLevel filter, const usize index)
    -> std::pair<LogLevel, const std::string&>
{
  usize i = 0;

  for (const auto& [level, str] : gHistory) {
    if (IsEnabled(filter, level)) {
      if (i == index) {
        return {level, str};
      }
      else {
        ++i;
      }
    }
  }

  ThrowTraced(TactileError{"Invalid index for filtered log entry!"});
}

auto GetLogSize(const LogLevel filter) -> usize
{
  usize count = 0;

  for (const auto& [level, str] : gHistory) {
    if (IsEnabled(filter, level)) {
      ++count;
    }
  }

  return count;
}

auto IsEnabled(const LogLevel filter, const LogLevel level) -> bool
{
  switch (level) {
    case LogLevel::Verbose:
      return filter == LogLevel::Verbose;

    case LogLevel::Debug:
      return filter == LogLevel::Verbose || filter == LogLevel::Debug;

    case LogLevel::Info:
      return filter == LogLevel::Verbose || filter == LogLevel::Debug ||
             filter == LogLevel::Info;

    case LogLevel::Warning:
      return filter == LogLevel::Verbose || filter == LogLevel::Debug ||
             filter == LogLevel::Info || filter == LogLevel::Warning;

    case LogLevel::Error:
      return true; /* Errors are always logged */

    default:
      return false;
  }
}

auto IsEnabled(const LogLevel level) -> bool
{
  return IsEnabled(gLogLevel, level);
}

}  // namespace tactile
