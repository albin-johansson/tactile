#pragma once

#include <filesystem>  // path
#include <string>      // string

#include <tactile-base/property_type.hpp>
#include <tactile-base/property_value.hpp>

#include "../../parse_ir.hpp"

namespace Tactile::IO {

constexpr auto tiled_version = "1.7.0";
constexpr auto tiled_xml_version = "1.5";
constexpr auto tiled_json_version = "1.6";

[[nodiscard]] auto ConvertToForwardSlashes(const std::filesystem::path& path)
    -> std::string;

[[nodiscard]] auto GetTilesetImagePath(const std::filesystem::path& image,
                                       const std::filesystem::path& dir) -> std::string;

[[nodiscard]] auto GetPropertyTypeString(PropertyType type) -> std::string;

[[nodiscard]] auto GetPropertyFileValue(const PropertyValue& file,
                                        const std::filesystem::path& dir) -> std::string;

[[nodiscard]] auto IsTileWorthSaving(const TileData& tile) -> bool;

}  // namespace Tactile::IO
