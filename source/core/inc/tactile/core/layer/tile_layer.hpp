// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/entity/entity.hpp"
#include "tactile/core/layer/dense_tile_matrix.hpp"
#include "tactile/core/layer/sparse_tile_matrix.hpp"
#include "tactile/core/util/matrix_extent.hpp"

namespace tactile {

class Registry;

/// \addtogroup TileLayer
/// \{

/**
 * Tag component for tile layers.
 */
struct CTileLayer final : CTag
{};

/**
 * A component used by tile layers that feature densely packed tile data.
 */
struct CDenseTileLayer final
{
  TACTILE_DEFAULT_ALL(CDenseTileLayer);

  DenseTileMatrix tiles;
};

/**
 * A component used by tile layers that feature sparsely packed tile data.
 */
struct CSparseTileLayer final
{
  TACTILE_DEFAULT_ALL(CSparseTileLayer);

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
 * \param entity   The entity to check.
 *
 * \return
 * True if the entity is a tile layer; false otherwise.
 */
[[nodiscard]]
auto is_tile_layer(const Registry& registry, EntityID entity) -> bool;

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
auto make_tile_layer(Registry& registry,
                     const MatrixExtent& extent) -> EntityID;

/**
 * Destroys a tile layer.
 *
 * \pre The specified entity must be a tile layer.
 *
 * \param registry     The associated registry.
 * \param layer_entity The tile layer entity to destroy.
 */
void destroy_tile_layer(Registry& registry, EntityID layer_entity);

/**
 * Makes a tile layer use a dense tile representation.
 *
 * \details
 * This has no effect if the tile layer is dense.
 *
 * \pre The specified entity must be a tile layer.
 *
 * \param registry     The associated registry.
 * \param layer_entity The target tile layer entity.
 */
void to_dense_tile_layer(Registry& registry, EntityID layer_entity);

/**
 * Makes a tile layer use a sparse tile representation.
 *
 * \details
 * This has no effect if the tile layer is sparse.
 *
 * \pre The specified entity must be a tile layer.
 *
 * \param registry     The associated registry.
 * \param layer_entity The target tile layer entity.
 */
void to_sparse_tile_layer(Registry& registry, EntityID layer_entity);

/// \}

}  // namespace tactile
