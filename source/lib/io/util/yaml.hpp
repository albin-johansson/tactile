// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // move

#include <yaml-cpp/yaml.h>

#include "core/layer/tile_format.hpp"
#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/core/debug/generic_error.hpp"
#include "tactile/base/meta/attribute.hpp"

namespace tactile {

auto save_yaml_to_file(const YAML::Emitter& emitter, const Path& path) -> Result<void>;

/// Reads an attribute to a destination variable.
template <typename T>
auto read_attr(const YAML::Node& node, const char* name, T& result) -> Result<void>
{
  if (auto attr = node[name]) {
    result = attr.as<T>();
    return kOK;
  }

  return unexpected(make_error(GenericError::kInvalidParam));
}

template <>
inline auto read_attr(const YAML::Node& node, const char* name, String& result)
    -> Result<void>
{
  if (auto attr = node[name]) {
    result = attr.as<String>();
    return kOK;
  }

  return unexpected(make_error(GenericError::kInvalidParam));
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
auto operator<<(YAML::Emitter& emitter, CompressionType compression) -> YAML::Emitter&;
auto operator<<(YAML::Emitter& emitter, AttributeType type) -> YAML::Emitter&;
auto operator<<(YAML::Emitter& emitter, const Attribute& value) -> YAML::Emitter&;

}  // namespace tactile
