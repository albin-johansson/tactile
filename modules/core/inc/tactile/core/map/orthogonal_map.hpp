// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/map/layer/group_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/map/tile_format.hpp"
#include "tactile/core/tileset/attached_tileset.hpp"
#include "tactile/foundation/container/vector.hpp"
#include "tactile/foundation/functional/maybe.hpp"
#include "tactile/foundation/math/vector.hpp"
#include "tactile/foundation/misc/id_types.hpp"
#include "tactile/foundation/misc/tile_matrix.hpp"
#include "tactile/foundation/misc/uuid.hpp"
#include "tactile/foundation/prelude.hpp"

namespace tactile {

/**
 * A map variant that uses plain rectangular tiles.
 */
class TACTILE_CORE_API OrthogonalMap final : public IMap {
 public:
  OrthogonalMap(const MatrixExtent& extent, const Int2& tile_size);

  void accept(IMetaContextVisitor& visitor) override;

  void set_extent(const MatrixExtent& extent) override;

  [[nodiscard]]
  auto extent() const -> const MatrixExtent& override;

  void set_tile_size(Int2 size) override;

  [[nodiscard]]
  auto tile_size() const -> Int2 override;

  [[nodiscard]]
  auto root_layer() -> GroupLayer& override;

  [[nodiscard]]
  auto root_layer() const -> const GroupLayer& override;

  [[nodiscard]]
  auto tile_format() -> TileFormat& override;

  [[nodiscard]]
  auto tile_format() const -> const TileFormat& override;

  [[nodiscard]]
  auto meta() -> Metadata& override;

  [[nodiscard]]
  auto meta() const -> const Metadata& override;

 private:
  MatrixExtent mExtent;
  Int2 mTileSize;
  GroupLayer mRootLayer {};
  Vector<AttachedTileset> mTilesets {};
  TileFormat mTileFormat {};
  Metadata mMeta {};
  Maybe<UUID> mActiveLayerUuid {};
  LayerID mNextLayerId {1};
  ObjectID mNextObjectId {1};
  int32 mNextTileLayerSuffix {1};
  int32 mNextObjectLayerSuffix {1};
  int32 mNextGroupLayerSuffix {1};
};

}  // namespace tactile
