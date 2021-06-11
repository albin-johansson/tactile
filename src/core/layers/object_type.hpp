#pragma once

namespace tactile::core {

/**
 * \enum ObjectType
 *
 * \brief Provides values that represent the various kinds of possible objects.
 *
 * \since 0.2.0
 *
 * \headerfile object_type.hpp
 */
enum class ObjectType
{
  Point,     ///< An object represented by a point.
  Rectangle  ///< An object represented by a rectangle.
};

}  // namespace tactile::core
