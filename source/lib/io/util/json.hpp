// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <nlohmann/json.hpp>

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/meta/attribute.hpp"

namespace tactile {

using JSON = nlohmann::json;

/// ADL functions for converting attribute types to/from JSON.
void to_json(JSON& json, AttributeType type);
void to_json(JSON& json, const Attribute& value);
void from_json(const JSON& json, AttributeType& type);

/// Parses the JSON file at the specified path, returning nothing if something goes wrong.
[[nodiscard]] auto parse_json_file(const Path& path) -> Maybe<JSON>;

/// Saves a JSON file to the specified path, returning failure if an error occurs.
auto save_json_to_file(const JSON& json, const Path& path) -> Result<void>;

/// Attempts to retrieve an attribute from a JSON object.
[[nodiscard]] auto try_get(const JSON& json, const char* key) -> const JSON*;

/// Attribute retrieval functions.
[[nodiscard]] auto as_string(const JSON& json, StringView name) -> Maybe<String>;
[[nodiscard]] auto as_int(const JSON& json, StringView name) -> Maybe<int32>;
[[nodiscard]] auto as_uint(const JSON& json, StringView name) -> Maybe<uint32>;
[[nodiscard]] auto as_float(const JSON& json, StringView name) -> Maybe<float>;
[[nodiscard]] auto as_bool(const JSON& json, StringView name) -> Maybe<bool>;

}  // namespace tactile
