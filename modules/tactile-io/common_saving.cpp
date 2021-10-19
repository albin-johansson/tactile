#include "common_saving.hpp"

#include <filesystem>  // relative

#include <tactile-base/convert_to_forward_slashes.hpp>
#include <tactile-base/tactile_error.hpp>

namespace Tactile::IO {

auto GetTilesetImagePath(const std::filesystem::path& image,
                         const std::filesystem::path& dir) -> std::string
{
  const auto path = std::filesystem::relative(image, dir);
  return ConvertToForwardSlashes(path);
}

auto GetPropertyTypeString(const PropertyType type) -> std::string
{
  switch (type) {
    case PropertyType::String:
      return "string";

    case PropertyType::Integer:
      return "int";

    case PropertyType::Floating:
      return "float";

    case PropertyType::Boolean:
      return "bool";

    case PropertyType::File:
      return "file";

    case PropertyType::Color:
      return "color";

    case PropertyType::Object:
      return "object";

    default:
      throw TactileError{"Could not recognize property type!"};
  }
}

auto GetPropertyFileValue(const PropertyValue& file, const std::filesystem::path& dir)
    -> std::string
{
  const auto abs = std::filesystem::absolute(dir / file.AsFile());
  const auto path = std::filesystem::relative(abs, dir);
  return ConvertToForwardSlashes(path);
}

auto IsTileWorthSaving(const TileData& tile) -> bool
{
  return !tile.animation.empty() || !tile.properties.empty();
}

}  // namespace Tactile::IO