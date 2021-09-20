#include "common_saving.hpp"

#include <algorithm>   // replace
#include <filesystem>  // relative

#include "core/tactile_error.hpp"

namespace Tactile::IO {

auto ConvertToForwardSlashes(const std::filesystem::path& path) -> std::string
{
  /* Here we make sure that the file path is portable, by using forward slashes that
     can be understood by pretty much all operating systems that we care about. */
  auto str = path.string();
  std::ranges::replace(str, '\\', '/');
  return str;
}

[[nodiscard]] auto GetTilesetImagePath(const std::filesystem::path& image,
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
  const auto path = std::filesystem::relative(file.AsFile(), dir);
  return ConvertToForwardSlashes(path);
}

}  // namespace Tactile::IO