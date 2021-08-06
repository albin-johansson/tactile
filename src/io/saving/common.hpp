#pragma once

#include <filesystem>  // path
#include <string>      // string

#include "core/properties/property_type.hpp"
#include "core/tileset/tileset.hpp"

namespace Tactile::IO {

constexpr auto tiled_version = "1.7.0";
constexpr auto tiled_xml_version = "1.5";
constexpr auto tiled_json_version = "1.6";

[[nodiscard]] auto ConvertToForwardSlashes(const std::filesystem::path& path)
    -> std::string;

[[nodiscard]] auto GetTilesetImagePath(const Tileset& tileset,
                                       const std::filesystem::path& dir)
    -> std::string;

[[nodiscard]] auto GetPropertyTypeString(PropertyType type) -> std::string;

[[nodiscard]] auto GetPropertyFileValue(const Property& file,
                                        const std::filesystem::path& dir)
    -> std::string;

}  // namespace Tactile::IO
