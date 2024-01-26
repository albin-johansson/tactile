// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/layer/layer_behavior_delegate.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/result.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"
#include "tactile/foundation/misc/matrix_extent.hpp"
#include "tactile/foundation/misc/tile_pos.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

class TACTILE_CORE_API TileLayer : public ILayer {
 public:
  void accept(IMetaContextVisitor& visitor) override;

  void accept(ILayerVisitor& visitor) override;

  void accept(IConstLayerVisitor& visitor) const override;

  /**
   * Updates the tile ID stored at the specified position.
   *
   * \note
   *    This function does nothing if the tile position is invalid.
   *
   * \param pos The tile position.
   * \param id  The new tile ID.
   *
   * \return
   *    Nothing if the tile was updated; an error code otherwise.
   */
  virtual auto set_tile(const TilePos& pos, TileID id) -> Result<void> = 0;

  /**
   * Returns the tile ID at the specified position.
   *
   * \param pos The tile position.
   *
   * \return
   *    The tile ID, or nothing if the position is invalid.
   */
  [[nodiscard]]
  virtual auto tile_at(const TilePos& pos) const -> Maybe<TileID> = 0;

  /**
   * Changes the size of the layer.
   *
   * \param extent The new extent, must be at least 1x1.
   *
   * \return
   *    Nothing on success; an error code otherwise.
   */
  virtual auto set_extent(const MatrixExtent& extent) -> Result<void> = 0;

  /**
   * Returns the dimensions of the tile layer.
   *
   * \return
   *    The layer extent.
   */
  [[nodiscard]]
  virtual auto extent() const -> MatrixExtent = 0;

  /**
   * Indicates whether the specified position is within the layer.
   *
   * \param pos The position to check.
   *
   * \return
   *    True if the position is valid for use with the layer; false otherwise.
   */
  [[nodiscard]]
  auto is_valid_pos(const TilePos& pos) const -> bool;

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
  auto meta() -> Metadata& override;

  [[nodiscard]]
  auto meta() const -> const Metadata& override;


 protected:
  LayerBehaviorDelegate mDelegate;
};

}  // namespace tactile
