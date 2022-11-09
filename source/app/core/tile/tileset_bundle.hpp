/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#pragma once

#include <boost/uuid/uuid_hash.hpp>

#include "core/tile/tileset.hpp"
#include "core/tile/tileset_ref.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/maybe.hpp"
#include "core/type/ptr.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

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

  [[nodiscard]] auto has_tileset(const UUID& id) const -> bool;

  /// Indicates whether there is a tileset with the specified tile identifier.
  [[nodiscard]] auto is_valid_tile(TileID id) const -> bool;

  /// Finds the tileset that contains a specific global tile identifier.
  /// \complexity O(1)
  [[nodiscard]] auto find_tileset(TileID tile) const -> Maybe<UUID>;

  [[nodiscard]] auto get_ref(const UUID& id) -> TilesetRef&;
  [[nodiscard]] auto get_ref(const UUID& id) const -> const TilesetRef&;

  [[nodiscard]] auto find_ref(const UUID& id) -> TilesetRef*;
  [[nodiscard]] auto find_ref(const UUID& id) const -> const TilesetRef*;

  [[nodiscard]] auto to_tile_index(TileID id) const -> TileIndex;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto active_tileset_id() const -> const Maybe<UUID>&;

  [[nodiscard]] auto next_tile_id() const noexcept -> TileID { return mNextTileId; }

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
