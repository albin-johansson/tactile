#pragma once

#include <filesystem>  // path
#include <string>      // string

#include <tactile_def.hpp>

#include "ir_animation_frame.hpp"

namespace tactile::IO {

constexpr auto tiled_version = "1.7.0";
constexpr auto tiled_xml_version = "1.5";
constexpr auto tiled_json_version = "1.6";

[[nodiscard]] auto GetTilesetImagePath(const std::filesystem::path& image,
                                       const std::filesystem::path& dir) -> std::string;

[[nodiscard]] auto GetPropertyTypeString(PropertyType type) -> std::string;

[[nodiscard]] auto GetPropertyFileValue(const Property& file,
                                        const std::filesystem::path& dir) -> std::string;

[[nodiscard]] auto AsRGB(const Color& color) -> std::string;

[[nodiscard]] auto AsRGBA(const Color& color) -> std::string;

[[nodiscard]] auto AsARGB(const Color& color) -> std::string;

}  // namespace tactile::IO
