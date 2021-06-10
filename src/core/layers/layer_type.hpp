#pragma once

namespace tactile::core {

/**
 * \enum LayerType
 *
 * \brief Provides different values that represent the various tile types.
 *
 * \since 0.2.0
 *
 * \headerfile layer_type.hpp
 */
enum class LayerType
{
  tile_layer,   ///< A layer consisting of only tiles.
  object_layer  ///< A layer consisting of map objects.
};

}  // namespace tactile::core
