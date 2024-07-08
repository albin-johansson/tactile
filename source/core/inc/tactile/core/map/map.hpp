// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/container/expected.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_type.hpp"
#include "tactile/base/layer/tile_encoding.hpp"
#include "tactile/base/layer/tile_orientation.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/matrix_extent.hpp"

namespace tactile {

struct MapSpec;
struct TilesetSpec;
class Registry;

/**
 * A component featured by all maps.
 */
struct CMap final
{
  /** The tile orientation used by the map. */
  TileOrientation orientation;

  /** The size of the map, in tiles. */
  MatrixExtent extent;

  /** The logical size of all tiles. */
  Int2 tile_size;

  /** The invisible root group layer. */
  EntityID root_layer;

  /** The active layer, if any. */
  EntityID active_layer;

  /** The active attached tileset, if any. */
  EntityID active_tileset;

  /** The attached tilesets. */
  Vector<EntityID> attached_tilesets;
};

/**
 * A component that provides information about the tile format used by a map.
 */
struct CTileFormat final
{
  /** The tile encoding strategy. */
  TileEncoding encoding;

  /** The compression strategy. */
  CompressionType compression;

  /** The compression level, if any. */
  Maybe<int> comp_level;
};

/**
 * A component that tracks available identifiers.
 */
struct CMapIdCache final
{
  /** The next available global tile ID. */
  TileID next_tile_id;

  /** The next available object identifier. */
  int32 next_object_id;

  /** The next available layer identifier. */
  int32 next_layer_id;
};

/**
 * A component that stores integer suffixes used in generated layer names.
 */
struct CLayerSuffixes final
{
  /** The next tile layer suffix. */
  int32 tile_layer_suffix;

  /** The next object layer suffix. */
  int32 object_layer_suffix;

  /** The next group layer suffix. */
  int32 group_layer_suffix;
};

/**
 * Indicates whether a given entity represents a map.
 *
 * \details
 * Map entities feature the following components. \n
 * - \c CMeta \n
 * - \c CMap \n
 * - \c CTileFormat \n
 * - \c CMapIdCache \n
 * - \c CLayerSuffixes \n
 * - \c CViewport
 *
 * \param registry The associated registry.
 * \param entity   The entity that will be checked.
 *
 * \return
 * True if the entity represents a map; false otherwise.
 */
[[nodiscard]]
auto is_map(const Registry& registry, EntityID entity) -> bool;

/**
 * Creates an empty map.
 *
 * \param registry The associated registry.
 * \param sec      The map specification.
 *
 * \return
 * A map entity if successful; an invalid entity otherwise.
 */
[[nodiscard]]
auto make_map(Registry& registry, const MapSpec& spec) -> EntityID;

/**
 * Destroys a map.
 *
 * \pre The specified entity must be a map.
 *
 * \param registry   The associated registry.
 * \param map_entity The map that will be destroyed.
 */
void destroy_map(Registry& registry, EntityID map_entity);

/**
 * Loads a tileset and adds it to a map.
 *
 * \details
 * This function will create a tileset instance and update the map's identifier
 * state cache. The newly created tileset will also automatically be made the
 * active tileset in the map.
 *
 * \pre The specified entity must be a map.
 *
 * \param registry     The associated registry.
 * \param map_entity   The map that will host the tileset.
 * \param tileset_spec The tileset specification.
 *
 * \return
 * Nothing if successful; an error code otherwise.
 */
[[nodiscard]]
auto add_tileset_to_map(Registry& registry,
                        EntityID map_entity,
                        const TilesetSpec& tileset_spec) -> Result<void>;

/// \}

}  // namespace tactile
