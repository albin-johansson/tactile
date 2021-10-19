#pragma once

#include <magic_enum.hpp>  // enum_name
#include <string_view>     // string_view

namespace Tactile {

/// \addtogroup core
/// \{

/// \brief Provides values for all different possible property types.
enum class PropertyType
{
  String,    ///< A string property.
  Integer,   ///< An integer property.
  Floating,  ///< A floating-point property.
  Boolean,   ///< A boolean property.
  File,      ///< A file path property.
  Color,     ///< A color property.
  Object     ///< An integer ID property, that refers to a map object.
};

/// \} End of group core

}  // namespace Tactile

namespace magic_enum::customize {

template <>
constexpr auto enum_name<Tactile::PropertyType>(Tactile::PropertyType value) noexcept
    -> std::string_view
{
  switch (value) {
    case Tactile::PropertyType::String:
      return "string";

    case Tactile::PropertyType::Integer:
      return "int";

    case Tactile::PropertyType::Floating:
      return "float";

    case Tactile::PropertyType::Boolean:
      return "bool";

    case Tactile::PropertyType::File:
      return "file";

    case Tactile::PropertyType::Color:
      return "color";

    case Tactile::PropertyType::Object:
      return "object";

    default:
      return {};
  }
}

}  // namespace magic_enum::customize