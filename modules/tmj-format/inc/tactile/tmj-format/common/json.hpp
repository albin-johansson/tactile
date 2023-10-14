// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <utility>  // move

#include <nlohmann/json.hpp>

#include "tactile/core/container/string.hpp"
#include "tactile/core/debug/log/logger.hpp"
#include "tactile/core/format/save_format_error.hpp"
#include "tactile/core/functional/result.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/tmj-format/api.hpp"

namespace tactile::tmj {

using JSON = nlohmann::json;

/**
 * \brief Attempts to parse a JSON document from disk.
 *
 * \param path the JSON file path.
 *
 * \return a JSON object, or an error code on failure.
 */
[[nodiscard]]
TACTILE_TMJ_API auto parse_json(const FilePath& path) -> Result<JSON>;

/**
 * \brief Attempts to extract an attribute in a JSON object.
 *
 * \param      json           the JSON object.
 * \param      attribute_name the name of the JSON attribute.
 * \param[out] value          the output variable.
 *
 * \return nothing on success, or an error code on failure.
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
  return error(SaveFormatError::kMissingKey);
}

/**
 * \brief Attempts to extract an attribute in a JSON object.
 *
 * \param      json           the JSON object.
 * \param      attribute_name the name of the JSON attribute.
 * \param[out] value          the output variable.
 * \param      fallback       the fallback value used if the attribute isn't found.
 *
 * \return nothing on success, or an error code on failure.
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

}  // namespace tactile::tmj
