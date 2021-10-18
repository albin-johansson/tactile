#include "common_saving.hpp"

#include <algorithm>   // replace
#include <cassert>     // assert
#include <filesystem>  // relative

#include <tactile-base/tactile_error.hpp>
#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/property_context.hpp"

namespace Tactile::IO {

auto ConvertToForwardSlashes(const std::filesystem::path& path) -> std::string
{
  /* Here we make sure that the file path is portable, by using forward slashes that
     can be understood by pretty much all operating systems that we care about. */
  auto str = path.string();
  std::ranges::replace(str, '\\', '/');
  return str;
}

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

auto IsTileWorthSaving(const entt::registry& registry, const entt::entity entity) -> bool
{
  assert(entity != entt::null);
  assert(registry.all_of<FancyTile>(entity));
  return registry.all_of<Animation>(entity) ||
         !registry.get<PropertyContext>(entity).properties.empty();
}

}  // namespace Tactile::IO