#include "logging.hpp"

#include <ctime>  // time
#include <deque>  // deque

#include <fmt/chrono.h>
#include <fmt/core.h>
#include <tactile_stdlib.hpp>

#include "throw.hpp"

namespace tactile {
namespace {

struct logged_string final
{
  log_level level{};
  std::string str;
};

inline std::deque<logged_string> _history;
inline log_level _log_level{log_level::info};

void _log(const fmt::color color,
          const log_level level,
          const std::string_view priority,
          const std::string_view fmt,
          const fmt::format_args args)
{
  const auto time = fmt::localtime(std::time(nullptr));
  const auto msg = fmt::vformat(fmt, args);
  const auto full = fmt::vformat("{:%H:%M:%S} > {}\n", fmt::make_format_args(time, msg));

  _history.push_back(logged_string{level, full});

  if constexpr (is_debug_build) {
    print(color, "{:>9} {}", priority, full);
  }
}

}  // namespace

namespace LoggerImpl {

void log_verbose_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(log_level::verbose)) {
    _log(fmt::color::violet, log_level::verbose, "[VERBOSE]", fmt, args);
  }
}

void log_debug_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(log_level::debug)) {
    _log(fmt::color::aquamarine, log_level::debug, "[DEBUG]", fmt, args);
  }
}

void log_info_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(log_level::info)) {
    _log(fmt::color::white, log_level::info, "[INFO]", fmt, args);
  }
}

void log_warning_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(log_level::warning)) {
    _log(fmt::color::yellow, log_level::warning, "[WARNING]", fmt, args);
  }
}

void log_error_v(const std::string_view fmt, const fmt::format_args args)
{
  if (is_enabled(log_level::error)) {
    _log(fmt::color::orange_red, log_level::error, "[ERROR]", fmt, args);
  }
}

}  // namespace LoggerImpl

void clear_log_history()
{
  _history.clear();
}

void set_log_level(const log_level level)
{
  _log_level = level;
}

auto get_filtered_log_entry(const log_level filter, const usize index)
    -> std::pair<log_level, const std::string&>
{
  usize i = 0;

  for (const auto& [level, str] : _history) {
    if (is_enabled(filter, level)) {
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

auto log_size(const log_level filter) -> usize
{
  usize count = 0;

  for (const auto& [level, str] : _history) {
    if (is_enabled(filter, level)) {
      ++count;
    }
  }

  return count;
}

auto is_enabled(const log_level filter, const log_level level) -> bool
{
  switch (level) {
    case log_level::verbose:
      return filter == log_level::verbose;

    case log_level::debug:
      return filter == log_level::verbose || filter == log_level::debug;

    case log_level::info:
      return filter == log_level::verbose || filter == log_level::debug ||
             filter == log_level::info;

    case log_level::warning:
      return filter == log_level::verbose || filter == log_level::debug ||
             filter == log_level::info || filter == log_level::warning;

    case log_level::error:
      return true; /* Errors are always logged */

    default:
      return false;
  }
}

auto is_enabled(const log_level level) -> bool
{
  return is_enabled(_log_level, level);
}

}  // namespace tactile
