// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <yaml-cpp/yaml.h>

#include "tactile/foundation/container/file_path.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/foundation/prelude.hpp"
#include "tactile/tactile-yml-format/api.hpp"

namespace tactile::yml_format {

[[nodiscard]]
TACTILE_YML_FORMAT_API auto parse_yaml_file(const FilePath& path) -> Result<YAML::Node>;

template <typename T>
[[nodiscard]] auto try_parse_to(const YAML::Node& node, const StringView key, T& value)
    -> bool
{
  if (const auto node_value = node[key]) {
    value = node_value.as<T>();
    return true;
  }

  return false;
}

template <typename T>
[[nodiscard]] auto parse_to(const YAML::Node& node, const StringView key, T& value)
    -> Result<void>
{
  if (try_parse_to(node, key, value)) {
    return kOK;
  }

  TACTILE_LOG_ERROR("Missing key '{}'", key);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

template <typename T>
[[nodiscard]] auto try_parse(const YAML::Node& node, const StringView key) -> Maybe<T>
{
  T value {};
  if (try_parse_to(node, key, value)) {
    return value;
  }

  return kNothing;
}

template <typename T>
[[nodiscard]] auto parse(const YAML::Node& node, const StringView key) -> Result<T>
{
  T value {};
  if (const auto result = parse_to(node, key, value)) {
    return value;
  }
  else {
    return propagate_unexpected(result);
  }
}

}  // namespace tactile::yml_format
