// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "core/tile/tileset.hpp"
#include "core/tile/tileset_ref.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

/// Manager class used by maps to track attached tilesets.
class TilesetBundle final {
 public:
  /// Adds a tileset to the bundle with an explicit first tile identifier.
  /// This function does not touch the next tile ID property, so use with care.
  void attach_tileset(Shared<Tileset> tileset, TileID first_tile_id, bool embedded);

  void attach_tileset(Shared<Tileset> tileset, bool embedded);

  void detach_tileset(const UUID& id);

  void select_tileset(const UUID& id);

  void set_next_tile_id(TileID next_id);

  [[nodiscard]] auto has_tileset(const UUID& id) const -> bool;

  /// Indicates whether there is a tileset with the specified tile identifier.
  [[nodiscard]] auto is_valid_tile(TileID id) const -> bool;

  /// Finds the tileset that contains a specific global tile identifier.
  /// \complexity O(1)
  [[nodiscard]] auto find_tileset(TileID tile) const -> Maybe<UUID>;

  [[nodiscard]] auto get_tileset_ref(const UUID& id) -> TilesetRef&;
  [[nodiscard]] auto get_tileset_ref(const UUID& id) const -> const TilesetRef&;

  [[nodiscard]] auto find_tileset_ref(const UUID& id) -> TilesetRef*;
  [[nodiscard]] auto find_tileset_ref(const UUID& id) const -> const TilesetRef*;

  [[nodiscard]] auto to_tile_index(TileID id) const -> TileIndex;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto get_active_tileset_id() const -> const Maybe<UUID>&;

  [[nodiscard]] auto get_next_tile_id() const noexcept -> TileID { return mNextTileId; }

  [[nodiscard]] auto begin() noexcept { return mRefs.begin(); }
  [[nodiscard]] auto begin() const noexcept { return mRefs.begin(); }

  [[nodiscard]] auto end() noexcept { return mRefs.end(); }
  [[nodiscard]] auto end() const noexcept { return mRefs.end(); }

 private:
  HashMap<UUID, TilesetRef> mRefs;
  HashMap<TileID, UUID> mTileCache;
  Maybe<UUID> mActiveTileset;
  TileID mNextTileId {1};
};

}  // namespace tactile
