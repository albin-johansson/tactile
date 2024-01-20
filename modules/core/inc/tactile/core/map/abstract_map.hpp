// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/core/api.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/map/map_identifiers.hpp"
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
 * Abstract base class that implements most of the map interface.
 */
class TACTILE_CORE_API AbstractMap : public IMap {
 protected:
  /**
   * Creates a map.
   *
   * \param extent    The initial extent.
   * \param tile_size The logical tile size.
   */
  AbstractMap(const MatrixExtent& extent, const Int2& tile_size);

 public:
  void set_extent(const MatrixExtent& extent) final;

  [[nodiscard]]
  auto extent() const -> const MatrixExtent& final;

  void set_tile_size(Int2 size) final;

  [[nodiscard]]
  auto tile_size() const -> Int2 final;

  void set_active_layer(Maybe<UUID> layer_uuid) final;

  [[nodiscard]]
  auto active_layer_uuid() const -> Maybe<UUID> final;

  [[nodiscard]]
  auto root_layer() -> GroupLayer& final;

  [[nodiscard]]
  auto root_layer() const -> const GroupLayer& final;

  [[nodiscard]]
  auto tile_format() -> TileFormat& final;

  [[nodiscard]]
  auto tile_format() const -> const TileFormat& final;

  [[nodiscard]]
  auto identifiers() -> MapIdentifiers& final;

  [[nodiscard]]
  auto identifiers() const -> const MapIdentifiers& final;

  [[nodiscard]]
  auto meta() -> Metadata& final;

  [[nodiscard]]
  auto meta() const -> const Metadata& final;

 protected:
  MatrixExtent mExtent;
  Int2 mTileSize;
  GroupLayer mRootLayer {};
  Vector<AttachedTileset> mTilesets {};
  TileFormat mTileFormat {};
  Metadata mMeta {};
  Maybe<UUID> mActiveLayerUuid {};
  MapIdentifiers mIdentifiers {};
  int32 mNextTileLayerSuffix {1};
  int32 mNextObjectLayerSuffix {1};
  int32 mNextGroupLayerSuffix {1};
};

}  // namespace tactile
