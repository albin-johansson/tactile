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

#include <filesystem>   // path
#include <string>       // string
#include <string_view>  // string_view

#include <nlohmann/json.hpp>

#include "core/attribute.hpp"
#include "tactile.hpp"

namespace tactile {

NLOHMANN_JSON_SERIALIZE_ENUM(AttributeType,
                             {
                                 {AttributeType::string, "string"},
                                 {AttributeType::integer, "int"},
                                 {AttributeType::floating, "float"},
                                 {AttributeType::boolean, "bool"},
                                 {AttributeType::object, "object"},
                                 {AttributeType::color, "color"},
                                 {AttributeType::file, "file"},
                             })

void to_json(nlohmann::json& json, const Attribute& value);

void write_json(const nlohmann::json& json, const std::filesystem::path& path);

[[nodiscard]] auto read_json(const std::filesystem::path& path) -> Maybe<nlohmann::json>;

[[nodiscard]] auto as_string(const nlohmann::json& json, std::string_view name)
    -> Maybe<std::string>;

[[nodiscard]] auto as_int(const nlohmann::json& json, std::string_view name)
    -> Maybe<int32>;

[[nodiscard]] auto as_uint(const nlohmann::json& json, std::string_view name)
    -> Maybe<uint32>;

[[nodiscard]] auto as_float(const nlohmann::json& json, std::string_view name)
    -> Maybe<float>;

[[nodiscard]] auto as_bool(const nlohmann::json& json, std::string_view name)
    -> Maybe<bool>;

}  // namespace tactile