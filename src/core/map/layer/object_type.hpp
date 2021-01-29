#pragma once

namespace tactile::core {

/**
 * \enum object_type
 *
 * \brief Provides values that represent the various kinds of possible objects.
 *
 * \since 0.2.0
 *
 * \headerfile object_type.hpp
 */
enum class object_type
{
  point,     ///< An object represented by a point.
  rectangle  ///< An object represented by a rectangle.
};

}  // namespace tactile::core
