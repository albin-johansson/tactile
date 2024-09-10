// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <concepts>    // invocable
#include <functional>  // less
#include <map>         // map
#include <optional>    // optional

#include "tactile/base/id.hpp"
#include "tactile/base/io/byte_stream.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/base/util/matrix_extent.hpp"
#include "tactile/base/util/matrix_index.hpp"
#include "tactile/base/util/tile_matrix.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/entity/registry.hpp"

namespace tactile {

using SparseTileMatrix = std::map<MatrixIndex, TileID, std::less<>>;

/**
 * Base component for tile layers.
 */
struct CTileLayer final
{
  MatrixExtent extent;
};

/**
 * Component for densely populated tile layers.
 */
struct CDenseTileLayer final
{
  /** The associated tile data. */
  TileMatrix tiles;
};

/**
 * Component for sparsely populated tile layers.
 */
struct CSparseTileLayer final
{
  /** The associated tile data. */
  SparseTileMatrix tiles;
};

/**
 * Indicates whether an entity is a tile layer.
 *
 * \details
 * Tile layer entities feature the following components. \n
 * - \c CMeta \n
 * - \c CLayer \n
 * - \c CTileLayer \n
 * - Either \c CDenseTileLayer or \c CSparseTileLayer
 *
 * \param registry The associated registry.
 * \param id       The identifier of the entity to check.
 *
 * \return
 * True if the entity is a tile layer; false otherwise.
 */
[[nodiscard]]
auto is_tile_layer(const Registry& registry, EntityID id) -> bool;

/**
 * Creates an empty (dense) tile layer.
 *
 * \param registry The associated registry.
 * \param extent   The initial extent.
 *
 * \return
 * A tile layer entity.
 */
[[nodiscard]]
auto make_tile_layer(Registry& registry, const MatrixExtent& extent) -> EntityID;

/**
 * Destroys a tile layer.
 *
 * \pre The tile layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The identifier of the tile layer to destroy.
 */
void destroy_tile_layer(Registry& registry, EntityID layer_id);

/**
 * Makes a tile layer use a dense tile representation.
 *
 * \details
 * This has no effect if the tile layer is dense.
 *
 * \pre The tile layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The target tile layer.
 */
void convert_to_dense_tile_layer(Registry& registry, EntityID layer_id);

/**
 * Makes a tile layer use a sparse tile representation.
 *
 * \details
 * This has no effect if the tile layer is sparse.
 *
 * \pre The tile layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The target tile layer.
 */
void convert_to_sparse_tile_layer(Registry& registry, EntityID layer_id);

/**
 * Changes the size of a tile layer.
 *
 * \pre The tile layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The target layer identifier.
 * \param extent   The new layer extent.
 */
void resize_tile_layer(Registry& registry, EntityID layer_id, const MatrixExtent& extent);

/**
 * Serializes the tile data associated with a tile layer as a byte stream.
 *
 * \details
 * This function can be used to obtain the raw tile data suitable for use in for save files
 * that use a non-plain text encoding (and possibly compression).
 *
 * \pre The layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The target layer identifier.
 *
 * \return
 * A stream of tile bytes.
 */
[[nodiscard]]
auto serialize_tile_layer(const Registry& registry, EntityID layer_id) -> ByteStream;

/**
 * Updates a tile at a given position in a tile layer.
 *
 * \details
 * This function has no effect if the tile position is invalid.
 *
 * \pre The layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The target layer identifier.
 * \param index    The target tile position.
 * \param tile_id  The new tile identifier.
 */
void set_layer_tile(Registry& registry,
                    EntityID layer_id,
                    const MatrixIndex& index,
                    TileID tile_id);

/**
 * Returns the value of a tile at a given position in a tile layer.
 *
 * \pre The layer identifier must be valid.
 *
 * \param registry The associated registry.
 * \param layer_id The target layer identifier.
 * \param index    The target tile position.
 *
 * \return
 * A tile identifier if successful; an empty optional otherwise.
 */
[[nodiscard]]
auto get_layer_tile(const Registry& registry, EntityID layer_id, const MatrixIndex& index)
    -> std::optional<TileID>;

/**
 * Indicates whether a matrix index is within the interval [{0, 0}, {rows, columns}).
 *
 * \param extent The extent that represents the target region.
 * \param index  The index to check.
 *
 * \return
 * True if the index is within the region; false otherwise.
 */
[[nodiscard]]
auto is_index_within_extent(const MatrixExtent& extent, const MatrixIndex& index) noexcept
    -> bool;

/**
 * Visits each tile in a tile layer within a given region.
 *
 * \pre The tile layer identifier must be valid.
 *
 * \tparam T A function object type.
 *
 * \param registry The associated registry.
 * \param layer_id The target tile layer identifier.
 * \param begin    The inclusive first (top-left) tile position.
 * \param end      The exclusive last (bottom-right) tile position.
 * \param callable The function object invoked for each tile in the region.
 */
template <std::invocable<const MatrixIndex&, TileID> T>
constexpr void each_layer_tile(const Registry& registry,
                               const EntityID layer_id,
                               const MatrixIndex& begin,
                               const MatrixIndex& end,
                               const T& callable)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (const auto& tile_layer = registry.get<CTileLayer>(layer_id);
      !is_index_within_extent(tile_layer.extent, begin) ||
      !is_index_within_extent(tile_layer.extent, MatrixIndex {end.row - 1, end.col - 1})) {
    return;
  }

  if (const auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    for (auto row = begin.row; row < end.row; ++row) {
      for (auto col = begin.col; col < end.col; ++col) {
        const MatrixIndex index {row, col};
        callable(index, dense->tiles[static_cast<std::size_t>(row)][static_cast<std::size_t>(col)]);
      }
    }
  }
  else {
    const auto& sparse = registry.get<CSparseTileLayer>(layer_id);
    for (auto row = begin.row; row < end.row; ++row) {
      for (auto col = begin.col; col < end.col; ++col) {
        const MatrixIndex index {row, col};

        const auto iter = sparse.tiles.find(index);
        callable(index, iter != sparse.tiles.end() ? iter->second : kEmptyTile);
      }
    }
  }
}

/**
 * Visits each tile in a tile layer.
 *
 * \pre The tile layer identifier must be valid.
 *
 * \tparam T A function object type.
 *
 * \param registry The associated registry.
 * \param layer_id The target tile layer identifier.
 * \param callable The function object invoked for each tile in the layer.
 */
template <std::invocable<const MatrixIndex&, TileID> T>
constexpr void each_layer_tile(const Registry& registry,
                               const EntityID layer_id,
                               const T& callable)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
  const auto& tile_layer = registry.get<CTileLayer>(layer_id);

  constexpr MatrixIndex begin {0, 0};
  const MatrixIndex end {tile_layer.extent.rows, tile_layer.extent.cols};

  each_layer_tile(registry, layer_id, begin, end, callable);
}

}  // namespace tactile
