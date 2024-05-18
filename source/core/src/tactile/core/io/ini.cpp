// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/io/ini.hpp"

#include <algorithm>  // all_of
#include <cctype>     // isspace
#include <fstream>    // ifstream
#include <string>     // getline
#include <utility>    // move

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/log/set_log_scope.hpp"
#include "tactile/core/util/string_ops.hpp"

namespace tactile {
namespace {

inline constexpr char kCommentToken = ';';
inline constexpr char kAssignmentToken = '=';
inline constexpr char kSectionOpenToken = '[';
inline constexpr char kSectionCloseToken = ']';

[[nodiscard]]
auto _parse_section_header_name(const StringView current_line, const int line_number)
    -> Result<String>
{
  TACTILE_ASSERT(current_line.starts_with(kSectionOpenToken));

  if (!current_line.ends_with(kSectionCloseToken)) {
    TACTILE_LOG_ERROR("Detected unterminated section (line {})", line_number);
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  TACTILE_ASSERT(current_line.size() >= 2);
  return String {current_line.substr(1, current_line.size() - 2)};
}

[[nodiscard]]
auto _parse_key_value_pair(const StringView current_line,
                           const int line_number,
                           IniSection& section) -> Result<void>
{
  const auto eq_pos = current_line.find_first_of(kAssignmentToken);

  if (eq_pos == String::npos || eq_pos == 0) {
    TACTILE_LOG_ERROR("Detected missing assignment ('{}') operator (line {})",
                      kAssignmentToken,
                      line_number);
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  auto key = trim_string(current_line.substr(0, eq_pos));
  if (key.empty()) {
    TACTILE_LOG_ERROR("Detected empty key (line {})", line_number);
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  auto value = trim_string(current_line.substr(eq_pos + 1));

  const auto value_is_just_whitespace = std::ranges::all_of(value, [](const char ch) {
    return std::isspace(static_cast<uchar>(ch));
  });

  if (value.empty() || value_is_just_whitespace) {
    TACTILE_LOG_ERROR("Detected empty value (line {})", line_number);
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  section.insert_or_assign(std::move(key), std::move(value));
  return kOK;
}

}  // namespace

auto parse_ini(const Path& path) -> Result<IniData>
{
  const SetLogScope log_scope {"INI"};
  TACTILE_LOG_DEBUG("Parsing INI from {}", path.string());

  IniData ini_data {};

  std::ifstream stream {path, std::ios::in};
  if (!stream.good()) {
    TACTILE_LOG_ERROR("Could not read file");
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  int line_number {0};
  String current_section {};
  String current_line {};

  current_line.reserve(64);
  while (std::getline(stream, current_line)) {
    ++line_number;

    // Ignore empty lines and those with comments.
    if (current_line.empty() || current_line.starts_with(kCommentToken)) {
      continue;
    }

    if (current_line.starts_with(kSectionOpenToken)) {
      if (auto section_name = _parse_section_header_name(current_line, line_number)) {
        current_section = std::move(*section_name);

        // This is needed to include empty sections.
        ini_data[current_section];
      }
      else {
        return propagate_unexpected(section_name);
      }
    }
    else {
      if (current_section.empty()) {
        TACTILE_LOG_ERROR("Detected entry without parent section (line {})", line_number);
        return unexpected(make_error(GenericError::kInvalidFile));
      }

      const auto result =
          _parse_key_value_pair(current_line, line_number, ini_data[current_section]);
      if (!result) {
        return propagate_unexpected(result);
      }
    }

    current_line.clear();
  }

  return ini_data;
}

}  // namespace tactile
