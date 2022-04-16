/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "json_utils.hpp"

#include <fstream>  // ifstream, ofstream
#include <iomanip>  // setw
#include <ios>      // ios

#include "core/utils/strings.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/throw.hpp"

namespace tactile {
namespace {

template <typename T>
[[nodiscard]] auto _as(const nlohmann::json& json, const std::string_view name)
    -> std::optional<T>
{
  const auto iter = json.find(name);
  if (iter != json.end()) {
    return iter->get<T>();
  }
  else {
    return std::nullopt;
  }
}

}  // namespace

void to_json(nlohmann::json& json, const Attribute& value)
{
  switch (value.type()) {
    case AttributeType::string:
      json = value.as_string();
      break;

    case AttributeType::integer:
      json = value.as_int();
      break;

    case AttributeType::floating:
      json = value.as_float();
      break;

    case AttributeType::boolean:
      json = value.as_bool();
      break;

    case AttributeType::file:
      json = convert_to_forward_slashes(value.as_file());
      break;

    case AttributeType::color:
      json = value.as_color().as_argb();
      break;

    case AttributeType::object:
      json = value.as_object();
      break;

    default:
      panic("Invalid attribute type!");
  }
}

void write_json(const nlohmann::json& json, const std::filesystem::path& path)
{
  std::ofstream stream{path, std::ios::out | std::ios::trunc};

  if (get_preferences().indent_output()) {
    stream << std::setw(2);
  }

  stream << json;
}

auto read_json(const std::filesystem::path& path) -> std::optional<nlohmann::json>
{
  try {
    std::ifstream stream{path, std::ios::in};

    nlohmann::json json;
    stream >> json;

    return json;
  }
  catch (...) {
    return std::nullopt;
  }
}

auto as_string(const nlohmann::json& json, const std::string_view name)
    -> std::optional<std::string>
{
  return _as<std::string>(json, name);
}

auto as_int(const nlohmann::json& json, const std::string_view name) -> std::optional<int32>
{
  return _as<int32>(json, name);
}

auto as_uint(const nlohmann::json& json, const std::string_view name) -> std::optional<uint32>
{
  return _as<uint32>(json, name);
}

auto as_float(const nlohmann::json& json, const std::string_view name) -> std::optional<float>
{
  return _as<float>(json, name);
}

auto as_bool(const nlohmann::json& json, const std::string_view name) -> std::optional<bool>
{
  return _as<bool>(json, name);
}

}  // namespace tactile
