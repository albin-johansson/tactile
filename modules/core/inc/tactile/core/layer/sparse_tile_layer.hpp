// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/foundation/container/hash_map.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/misc/tile_pos.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * A tile layer variant optimized for sparse tile content.
 *
 * \details
 *    Sparse tile layers use hash maps as their representation of the
 *    logical tile matrix. This allows for a more compact representation
 *    of sparsely populated tile layers. Read and write access is not as
 *    efficient as with dense tile layers, but still good. It's important
 *    to note that sparse tile layers will only consume less memory
 *    compared to their dense counterparts if the content is truly sparse.
 *
 * \see DenseTileLayer
 */
class TACTILE_CORE_API SparseTileLayer final : public TileLayer {
 public:
  /**
   * Creates an empty sparse tile layer.
   *
   * \param row_count The number of tile rows, must be greater than zero.
   * \param col_count The number of tile columns, must be greater than zero.
   */
  SparseTileLayer(ssize row_count, ssize col_count);

  /**
   * Creates an empty sparse tile layer.
   *
   * \param extent The initial extent, must be at least 1x1.
   */
  explicit SparseTileLayer(MatrixExtent extent);

  auto set_tile(const TilePos& pos, TileID id) -> Result<void> override;

  [[nodiscard]]
  auto tile_at(const TilePos& pos) const -> Maybe<TileID> override;

  auto set_extent(const MatrixExtent& extent) -> Result<void> override;

  [[nodiscard]]
  auto extent() const -> MatrixExtent override;

  [[nodiscard]]
  auto clone() const -> Shared<ILayer> override;

 private:
  MatrixExtent mExtent;
  HashMap<TilePos, TileID> mTiles {};
};

}  // namespace tactile
