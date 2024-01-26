// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/layer_behavior_delegate.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/misc/tile_pos.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * A layer variant consisting of a two-dimensional grid of tile identifiers.
 */
class TACTILE_CORE_API TileLayer final : public ILayer {
 public:
  /**
   * Creates an empty tile layer.
   *
   * \param row_count The number of tile rows, must be greater than zero.
   * \param col_count The number of tile columns, must be greater than zero.
   */
  TileLayer(ssize row_count, ssize col_count);

  /**
   * Creates an empty tile layer.
   *
   * \param extent The initial extent, must be at least 1x1.
   */
  explicit TileLayer(MatrixExtent extent);

  void accept(IMetaContextVisitor& visitor) override;

  void accept(ILayerVisitor& visitor) override;

  void accept(IConstLayerVisitor& visitor) const override;

  /**
   * Changes the size of the layer.
   *
   * \param extent The new extent, must be at least 1x1.
   *
   * \return
   *    Nothing on success; an error code otherwise.
   */
  auto set_extent(const MatrixExtent& extent) -> Result<void>;

  /**
   * Updates the tile ID stored at the specified position.
   *
   * \note
   *    This function does nothing if the tile position is invalid.
   *
   * \param pos The tile position.
   * \param id  The new tile ID.
   */
  void set_tile(const TilePos& pos, TileID id);

  /**
   * Returns the tile ID at the specified position.
   *
   * \param pos The tile position.
   *
   * \return
   *    The tile ID, or nothing if the position is invalid.
   */
  [[nodiscard]]
  auto get_tile(const TilePos& pos) const -> Maybe<TileID>;

  /**
   * Indicates whether the specified position refers to a tile in the layer.
   *
   * \param pos The tile position.
   *
   * \return
   *    True if the position is valid for use with the layer; false otherwise.
   */
  [[nodiscard]]
  auto is_valid_pos(const TilePos& pos) const -> bool;

  /**
   * Returns the dimensions of the tile layer.
   *
   * \return
   *    The layer extent.
   */
  [[nodiscard]]
  auto extent() const -> MatrixExtent;

  void set_persistent_id(Maybe<int32> id) override;

  void set_opacity(float opacity) override;

  void set_visible(bool visible) override;

  [[nodiscard]]
  auto get_persistent_id() const -> Maybe<int32> override;

  [[nodiscard]]
  auto get_opacity() const -> float override;

  [[nodiscard]]
  auto is_visible() const -> bool override;

  [[nodiscard]]
  auto clone() const -> Shared<ILayer> override;

  [[nodiscard]]
  auto meta() -> Metadata& override;

  [[nodiscard]]
  auto meta() const -> const Metadata& override;

  template <std::invocable<const TilePos&, TileID> T>
  void each(const T& callable) const
  {
    for (ssize row = 0; row < mExtent.row_count; ++row) {
      for (ssize col = 0; col < mExtent.col_count; ++col) {
        callable(TilePos {row, col}, mTileMatrix[as_unsigned(row)][as_unsigned(col)]);
      }
    }
  }

 private:
  LayerBehaviorDelegate mDelegate;
  MatrixExtent mExtent;
  TileMatrix mTileMatrix;

  void _add_row();

  void _add_column();

  void _remove_row();

  void _remove_column();
};

}  // namespace tactile
