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

#include <string>       // string
#include <string_view>  // string_view

#include <nlohmann/json.hpp>

#include "core/attribute.hpp"
#include "core/common/fs.hpp"
#include "core/common/maybe.hpp"
#include "core/common/vocabulary.hpp"

namespace tactile {

using JSON = nlohmann::json;

NLOHMANN_JSON_SERIALIZE_ENUM(AttributeType,
                             {
                                 {AttributeType::String, "string"},
                                 {AttributeType::Int, "int"},
                                 {AttributeType::Float, "float"},
                                 {AttributeType::Bool, "bool"},
                                 {AttributeType::Object, "object"},
                                 {AttributeType::Color, "color"},
                                 {AttributeType::Path, "file"},
                             })

void to_json(JSON& json, const Attribute& value);

void write_json(const JSON& json, const fs::path& path);

[[nodiscard]] auto try_get(const JSON& json, const char* key) -> const JSON*;

[[nodiscard]] auto read_json(const fs::path& path) -> Maybe<JSON>;

namespace io {

[[nodiscard]] auto as_string(const JSON& json, std::string_view name)
    -> Maybe<std::string>;

[[nodiscard]] auto as_int(const JSON& json, std::string_view name) -> Maybe<int32>;

[[nodiscard]] auto as_uint(const JSON& json, std::string_view name) -> Maybe<uint32>;

[[nodiscard]] auto as_float(const JSON& json, std::string_view name) -> Maybe<float>;

[[nodiscard]] auto as_bool(const JSON& json, std::string_view name) -> Maybe<bool>;

}  // namespace io
}  // namespace tactile