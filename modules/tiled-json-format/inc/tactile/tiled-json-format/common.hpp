// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // move

#include <nlohmann/json.hpp>

#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/io/filesystem.hpp"
#include "tactile/foundation/io/save/save_format_error.hpp"
#include "tactile/foundation/log/logger.hpp"
#include "tactile/tiled-json-format/api.hpp"

namespace tactile::tiled::tmj {

using JSON = nlohmann::json;

/**
 * Attempts to parse a JSON document from disk.
 *
 * \param path The JSON file path.
 *
 * \return
 *    A JSON object, or an error code on failure.
 */
[[nodiscard]]
TACTILE_TMJ_FORMAT_API auto parse_json(const FilePath& path) -> Result<JSON>;

/**
 * Attempts to extract an attribute in a JSON object.
 *
 * \param      json           The JSON object.
 * \param      attribute_name The name of the JSON attribute.
 * \param[out] value          The output variable.
 *
 * \return
 *    Nothing on success, or an error code on failure.
 */
template <typename T>
[[nodiscard]] auto parse(const JSON& json, const StringView attribute_name, T& value)
    -> Result<void>
{
  if (const auto iter = json.find(attribute_name); iter != json.end()) {
    iter->get_to(value);
    return {};
  }

  TACTILE_LOG_ERROR("[TMJ] Missing required JSON key '{}'", attribute_name);
  return unexpected(make_save_format_error(SaveFormatError::kMissingKey));
}

/**
 * Attempts to extract an attribute in a JSON object.
 *
 * \param      json           The JSON object.
 * \param      attribute_name The name of the JSON attribute.
 * \param[out] value          The output variable.
 * \param      fallback       The fallback value used if the attribute isn't found.
 *
 * \return
 *    Nothing on success, or an error code on failure.
 */
template <typename T, typename U>
[[nodiscard]] auto parse(const JSON& json,
                         const StringView attribute_name,
                         T& value,
                         U fallback) -> Result<void>
{
  if (const auto iter = json.find(attribute_name); iter != json.end()) {
    iter->get_to(value);
    return {};
  }

  value = std::move(fallback);
  return {};
}

}  // namespace tactile::tiled::tmj
