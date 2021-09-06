#pragma once

#include <string>  // string

#include "aliases/object_id.hpp"
#include "core/object_type.hpp"

namespace Tactile {

struct Object final
{
  ObjectID id;              ///< Unique object identifier.
  float x{};                ///< X-coordinate.
  float y{};                ///< Y-coordinate.
  float width{};            ///< Object width (might be zero for points, etc.).
  float height{};           ///< Object height (might be zero for points, etc.).
  ObjectType type{};        ///< Specific object type.
  std::string custom_type;  ///< Optional user-provided type.
  bool visible{};           ///< Whether or not the object is rendered.
};

}  // namespace Tactile
