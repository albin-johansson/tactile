/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#pragma once

#include <utility>  // move

#include <yaml-cpp/yaml.h>

#include "common/type/path.hpp"
#include "core/attributes/attribute.hpp"
#include "tactile/core/containers/string.hpp"
#include "core/enums/tile_compression.hpp"
#include "core/enums/tile_encoding.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "core/functional/result.hpp"

namespace tactile {

auto save_yaml_to_file(const YAML::Emitter& emitter, const Path& path) -> Result;

/// Reads an attribute to a destination variable, returns false on failure.
template <typename T>
auto read_attr(const YAML::Node& node, const char* name, T& result) -> Result
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
    return success;
  }
  else {
    return failure;
  }
}

template <>
inline auto read_attr(const YAML::Node& node, const char* name, String& result) -> Result
{
  if (auto attr = node[name]) {
    result = attr.as<String>();
    return success;
  }
  else {
    return failure;
  }
}

/// Reads an attribute to a destination variable, uses the fallback value on failure.
template <typename T>
void read_attr_or(const YAML::Node& node, const char* name, T& result, T fallback)
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
  }
  else {
    result = std::move(fallback);
  }
}

template <>
inline void read_attr_or(const YAML::Node& node,
                         const char* name,
                         String& result,
                         String fallback)
{
  if (auto attr = node[name]) {
    result = attr.as<String>();
  }
  else {
    result = std::move(fallback);
  }
}

template <typename T>
void read_attr_or(const YAML::Node& node, const char* name, Maybe<T>& result, T fallback)
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
  }
  else {
    result = std::move(fallback);
  }
}
template <>
inline void read_attr_or(const YAML::Node& node,
                         const char* name,
                         Maybe<String>& result,
                         String fallback)
{
  if (auto attr = node[name]) {
    result = attr.as<String>();
  }
  else {
    result = std::move(fallback);
  }
}

auto operator<<(YAML::Emitter& emitter, TileEncoding encoding) -> YAML::Emitter&;
auto operator<<(YAML::Emitter& emitter, TileCompression compression) -> YAML::Emitter&;
auto operator<<(YAML::Emitter& emitter, AttributeType type) -> YAML::Emitter&;
auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&;

}  // namespace tactile
