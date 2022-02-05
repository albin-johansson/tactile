#pragma once

#include <filesystem>   // path
#include <string>       // string
#include <string_view>  // string_view

#include <nlohmann/json.hpp>

#include "core/attribute_value.hpp"
#include "tactile.hpp"

namespace tactile {

NLOHMANN_JSON_SERIALIZE_ENUM(attribute_type,
                             {
                                 {attribute_type::string, "string"},
                                 {attribute_type::integer, "int"},
                                 {attribute_type::floating, "float"},
                                 {attribute_type::boolean, "bool"},
                                 {attribute_type::object, "object"},
                                 {attribute_type::color, "color"},
                                 {attribute_type::file, "file"},
                             })

void to_json(nlohmann::json& json, const attribute_value& value);

void write_json(const nlohmann::json& json, const std::filesystem::path& path);

[[nodiscard]] auto read_json(const std::filesystem::path& path) -> maybe<nlohmann::json>;

[[nodiscard]] auto as_string(const nlohmann::json& json, std::string_view name)
    -> maybe<std::string>;

[[nodiscard]] auto as_int(const nlohmann::json& json, std::string_view name)
    -> maybe<int32>;

[[nodiscard]] auto as_uint(const nlohmann::json& json, std::string_view name)
    -> maybe<uint32>;

[[nodiscard]] auto as_float(const nlohmann::json& json, std::string_view name)
    -> maybe<float>;

[[nodiscard]] auto as_bool(const nlohmann::json& json, std::string_view name)
    -> maybe<bool>;

}  // namespace tactile