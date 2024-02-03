// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/io/ini.hpp"

#include <fstream>  // ifstream
#include <string>   // getline
#include <utility>  // move

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/generic_error.hpp"

namespace tactile {
namespace {

[[nodiscard]]
auto _parse_section_header_name(StringView current_line) -> Result<String>
{
  TACTILE_ASSERT(current_line.starts_with('['));
  if (!current_line.ends_with(']')) {
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  TACTILE_ASSERT(current_line.size() >= 2);
  return String {current_line.substr(1, current_line.size() - 2)};
}

[[nodiscard]]
auto _parse_key_value_pair(const StringView current_line, IniSection& section)
    -> Result<void>
{
  const auto eq_pos = current_line.find_first_of('=');

  if (eq_pos == String::npos || eq_pos == 0) {
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  String key {current_line.substr(0, eq_pos)};
  str_trim(key);

  if (key.empty()) {
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  String value {current_line.substr(eq_pos + 1)};
  str_trim(value);

  if (value.empty()) {
    return unexpected(make_error(GenericError::kInvalidFile));
  }

  section.insert_or_assign(std::move(key), std::move(value));
  return kOK;
}

}  // namespace

auto parse_ini(const FilePath& path) -> Result<IniData>
{
  IniData ini {};

  std::ifstream stream {path, std::ios::in};
  if (!stream.good()) {
    return unexpected(make_error(GenericError::kNoSuchFile));
  }

  String current_section {};
  String current_line {};

  current_line.reserve(64);
  while (std::getline(stream, current_line)) {
    if (current_line.empty() || current_line.starts_with(';')) {
      continue;
    }

    if (current_line.starts_with('[')) {
      if (auto section_name = _parse_section_header_name(current_line)) {
        current_section = std::move(*section_name);

        // This is needed to include empty sections.
        ini[current_section];
      }
      else {
        return propagate_unexpected(section_name);
      }
    }
    else {
      if (current_section.empty()) {
        return unexpected(make_error(GenericError::kInvalidFile));
      }

      const auto result = _parse_key_value_pair(current_line, ini[current_section]);
      if (!result) {
        return propagate_unexpected(result);
      }
    }

    current_line.clear();
  }

  return ini;
}

}  // namespace tactile
