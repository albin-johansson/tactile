// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/map/layer/layer.hpp"
#include "tactile/core/map/layer/layer_behavior_delegate.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/misc/id_types.hpp"
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

  void accept(IMetaContextVisitor& visitor) override;

  void accept(ILayerVisitor& visitor) override;

  void accept(IConstLayerVisitor& visitor) const override;

  /**
   * Changes the size of the layer.
   *
   * \param row_count The number of tile rows, must be greater than zero.
   * \param col_count The number of tile columns, must be greater than zero.
   */
  void resize(ssize row_count, ssize col_count);

  /**
   * Applies a flood fill algorithm to the layer.
   *
   * \param      start_pos          The flood start position.
   * \param      new_id             The replacement tile identifier.
   * \param[out] affected_positions A vector to which all modified positions are recorded.
   *
   * \see https://en.wikipedia.org/wiki/Flood_fill
   */
  void flood(const TilePos& start_pos,
             TileID new_id,
             Vector<TilePos>* affected_positions = nullptr);

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
   * Returns the number of tile rows stored in the layer.
   *
   * \return
   *    The tile row count.
   */
  [[nodiscard]]
  auto row_count() const -> ssize;

  /**
   * Returns the number of tile columns stored in the layer.
   *
   * \return
   *    The tile column count.
   */
  [[nodiscard]]
  auto column_count() const -> ssize;

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

 private:
  LayerBehaviorDelegate mDelegate;
  ssize mRowCount;
  ssize mColCount;
  TileMatrix mTileMatrix;

  void _add_row();

  void _add_column();

  void _remove_row();

  void _remove_column();
};

}  // namespace tactile
