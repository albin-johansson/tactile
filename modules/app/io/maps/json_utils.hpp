#pragma once

#include <filesystem>  // path

#include <nlohmann/json.hpp>

#include "core/attribute_value.hpp"
#include "tactile_def.hpp"

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

}  // namespace tactile