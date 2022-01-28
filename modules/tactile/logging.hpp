#pragma once

#include <string>       // string
#include <string_view>  // string_view
#include <utility>      // pair

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>  // included so that formatting types like std::filesystem::path works

#include "build.hpp"
#include "tactile_def.hpp"

namespace tactile {
namespace logger {

void log_verbose_v(std::string_view fmt, fmt::format_args args);
void log_debug_v(std::string_view fmt, fmt::format_args args);
void log_info_v(std::string_view fmt, fmt::format_args args);
void log_warning_v(std::string_view fmt, fmt::format_args args);
void log_error_v(std::string_view fmt, fmt::format_args args);

}  // namespace logger

enum class log_level {
  verbose,  ///< Logs everything.
  debug,    ///< Logs stuff that a developer might be interested in.
  info,     ///< Default log level, emit informational stuff for ordinary users.
  warning,  ///< Only emit warnings and errors.
  error     ///< Only emit errors.
};

template <typename... Args>
void print([[maybe_unused]] const fmt::color color,
           const std::string_view fmt,
           const Args&... args)
{
  if constexpr (on_windows) {
    fmt::print(fmt, args...);
  }
  else {
    fmt::print(fmt::fg(color), fmt, args...);
  }
}

template <typename... Args>
void log_verbose(const std::string_view fmt, const Args&... args)
{
  logger::log_verbose_v(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void log_debug(const std::string_view fmt, const Args&... args)
{
  logger::log_debug_v(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void log_info(const std::string_view fmt, const Args&... args)
{
  logger::log_info_v(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void log_warning(const std::string_view fmt, const Args&... args)
{
  logger::log_warning_v(fmt, fmt::make_format_args(args...));
}

template <typename... Args>
void log_error(const std::string_view fmt, const Args&... args)
{
  logger::log_error_v(fmt, fmt::make_format_args(args...));
}

/**
 * \brief Clears the entire log history.
 */
void clear_log_history();

void set_log_level(log_level level);

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
 * \see log_size(log_level)
 */
[[nodiscard]] auto get_filtered_log_entry(log_level filter, usize index)
    -> std::pair<log_level, const std::string&>;

/**
 * \brief Returns the amount of log entries that satisfy a filter.
 *
 * \param filter the filter that will be used.
 *
 * \return the amount of log entries that weren't filtered out.
 */
[[nodiscard]] auto log_size(log_level filter) -> usize;

/**
 * \brief Indicates whether a message using a specific log level satisfies a filter.
 *
 * \param filter the filter level to use.
 * \param level the log level to check.
 *
 * \return `true` if the log level is enabled according to the filter; `false` otherwise.
 */
[[nodiscard]] auto is_enabled(log_level filter, log_level level) -> bool;

[[nodiscard]] auto is_enabled(log_level level) -> bool;

}  // namespace tactile
