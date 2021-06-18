#pragma once

namespace tactile::core {

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

}  // namespace tactile::core
