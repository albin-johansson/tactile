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

#include <nlohmann/json.hpp>

#include "common/type/path.hpp"
#include "core/attributes/attribute.hpp"
#include "tactile/core/containers/string.hpp"
#include "tactile/core/functional/maybe.hpp"
#include "core/functional/result.hpp"
#include "core/prelude.hpp"

namespace tactile {

using JSON = nlohmann::json;

/// ADL functions for converting attribute types to/from JSON.
void to_json(JSON& json, AttributeType type);
void to_json(JSON& json, const Attribute& value);
void from_json(const JSON& json, AttributeType& type);

/// Parses the JSON file at the specified path, returning nothing if something goes wrong.
[[nodiscard]] auto parse_json_file(const Path& path) -> Maybe<JSON>;

/// Saves a JSON file to the specified path, returning failure if an error occurs.
auto save_json_to_file(const JSON& json, const Path& path, int indentation = 0) -> Result;

/// Attempts to retrieve an attribute from a JSON object.
[[nodiscard]] auto try_get(const JSON& json, const char* key) -> const JSON*;

/// Attribute retrieval functions.
[[nodiscard]] auto as_string(const JSON& json, StringView name) -> Maybe<String>;
[[nodiscard]] auto as_int(const JSON& json, StringView name) -> Maybe<int32>;
[[nodiscard]] auto as_uint(const JSON& json, StringView name) -> Maybe<uint32>;
[[nodiscard]] auto as_float(const JSON& json, StringView name) -> Maybe<float>;
[[nodiscard]] auto as_bool(const JSON& json, StringView name) -> Maybe<bool>;

}  // namespace tactile