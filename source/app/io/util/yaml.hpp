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

#pragma once

#include <utility>  // move

#include <yaml-cpp/yaml.h>

#include "core/attribute.hpp"
#include "core/layer/tile_format.hpp"
#include "core/type/maybe.hpp"
#include "core/type/string.hpp"

namespace tactile::io {

/// Reads an attribute to a destination variable, returns false on failure.
template <typename T>
auto read_attribute(const YAML::Node& node, const char* name, T& result) -> bool
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
    return true;
  }
  else {
    return false;
  }
}

template <>
inline auto read_attribute(const YAML::Node& node, const char* name, String& result)
    -> bool
{
  if (auto attr = node[name]) {
    result = attr.as<String>();
    return true;
  }
  else {
    return false;
  }
}

/// Reads an attribute to a destination variable, uses the fallback value on failure.
template <typename T>
void read_attribute(const YAML::Node& node, const char* name, T& result, T fallback)
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
  }
  else {
    result = std::move(fallback);
  }
}

template <>
inline void read_attribute(const YAML::Node& node,
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
void read_attribute(const YAML::Node& node,
                    const char* name,
                    Maybe<T>& result,
                    T fallback)
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
  }
  else {
    result = std::move(fallback);
  }
}
template <>
inline void read_attribute(const YAML::Node& node,
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

auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, AttributeType type) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, TileEncoding encoding) -> YAML::Emitter&;

auto operator<<(YAML::Emitter& emitter, TileCompression compression) -> YAML::Emitter&;

}  // namespace tactile::io
