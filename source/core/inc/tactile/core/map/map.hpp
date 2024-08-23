// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <expected>      // expected
#include <optional>      // optional
#include <system_error>  // error_code
#include <vector>        // vector

#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/io/compress/compression_format.hpp"
#include "tactile/base/layer/layer_type.hpp"
#include "tactile/base/layer/tile_encoding.hpp"
#include "tactile/base/layer/tile_orientation.hpp"
#include "tactile/base/numeric/vec.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"
#include "tactile/core/entity/entity.hpp"

namespace tactile {

struct MapSpec;
struct TilesetSpec;
class Registry;
class IRenderer;

namespace ir {
struct Map;
}  // namespace ir

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
  std::vector<EntityID> attached_tilesets;
};

/**
 * A component that provides information about the tile format used by a map.
 */
struct CTileFormat final
{
  /** The tile encoding strategy. */
  TileEncoding encoding;

  /** The compression strategy. */
  std::optional<CompressionFormat> compression;

  /** The compression level, if any. */
  std::optional<int> comp_level;
};

/**
 * A component that tracks available identifiers.
 */
struct CMapIdCache final
{
  /** The next available global tile ID. */
  TileID next_tile_id;

  /** The next available object identifier. */
  ObjectID next_object_id;

  /** The next available layer identifier. */
  LayerID next_layer_id;
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
 * \param spec     The map specification.
 *
 * \return
 * A map entity if successful; an invalid entity otherwise.
 */
[[nodiscard]]
auto make_map(Registry& registry, const MapSpec& spec) -> EntityID;

/**
 * Creates a map based on an intermediate representation.
 *
 * \param registry The associated registry.
 * \param renderer The renderer used to load textures.
 * \param ir_map   The intermediate map representation.
 *
 * \return
 * A map entity identifier if successful; an error code otherwise.
 */
[[nodiscard]]
auto make_map(Registry& registry,
              IRenderer& renderer,
              const ir::Map& ir_map) -> std::expected<EntityID, std::error_code>;

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
 * \param map_id       The map that will host the tileset.
 * \param tileset_spec The tileset specification.
 *
 * \return
 * A tileset entity identifier if successful; an error code otherwise.
 */
[[nodiscard]]
auto add_tileset_to_map(Registry& registry, EntityID map_id, const TilesetSpec& tileset_spec)
    -> std::expected<EntityID, std::error_code>;

/**
 * Removes a tileset from a map, but does not destroy it.
 *
 * \details
 * The tileset identifier will be removed from the map. If the removed tileset is active,
 * another tileset will be selected. Beware that the tileset is still kept in the registry, so
 * subsequent tileset component queries will include the tileset removed from the map.
 *
 * \pre The map identifier must be valid.
 * \pre The tileset identifier must be valid.
 *
 * \param registry   The associated registry.
 * \param map_id     The host map entity identifier.
 * \param tileset_id The target tileset entity identifier.
 */
void remove_tileset_from_map(Registry& registry, EntityID map_id, EntityID tileset_id);

/**
 * Creates a layer and adds it to a map.
 *
 * \pre The map identifier must be valid.
 *
 * \details
 * This function automatically makes the new layer the active layer in the map.
 *
 * \param registry The associated registry.
 * \param map_id   The target map identifier.
 * \param type     The type of the new layer.
 *
 * \return
 * A layer entity identifier if successful; an error code otherwise.
 */
[[nodiscard]]
auto add_layer_to_map(Registry& registry,
                      EntityID map_id,
                      LayerType type) -> std::expected<EntityID, std::error_code>;

/**
 * Appends an existing layer to a map.
 *
 * \details
 * If the currently active layer is a group layer, then the layer is appended to that group.
 * Otherwise, the layer is appended to the root layer. The new layer is subsequently made
 * active.
 *
 * \pre The map identifier must be valid.
 * \pre The layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param map_id   The host map identifier.
 * \param layer_id The identifier of the layer that will be added.
 */
void append_layer_to_map(Registry& registry, EntityID map_id, EntityID layer_id);

/**
 * Removes a layer from a map, but does not destroy it.
 *
 * \pre The map identifier must be valid.
 * \pre The layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param map_id   The host map identifier.
 * \param layer_id The target layer identifier.
 *
 * \return
 * Nothing if the layer was removed; an error code otherwise.
 */
[[nodiscard]]
auto remove_layer_from_map(Registry& registry,
                           EntityID map_id,
                           EntityID layer_id) -> std::expected<void, std::error_code>;

}  // namespace tactile
