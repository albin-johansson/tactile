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

#include "core/common/associative.hpp"
#include "core/common/identifiers.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/tilesets/tileset.hpp"
#include "core/tilesets/tileset_ref.hpp"

namespace tactile::core {

/// Manager class used by maps to track attached tilesets.
class TilesetBundle final
{
 public:
  /// Adds a tileset to the bundle with an explicit first tile identifier.
  /// This function does not touch the next tile ID property, so use with care.
  void attach_tileset(Shared<Tileset> tileset, TileID firstTileId, bool embedded);

  void attach_tileset(Shared<Tileset> tileset, bool embedded);

  void detach_tileset(const UUID& id);

  void select_tileset(const UUID& id);

  [[nodiscard]] auto has_tileset(const UUID& id) const -> bool;

  /// Indicates whether there is a tileset with the specified tile identifier.
  [[nodiscard]] auto is_valid_tile(TileID id) const -> bool;

  [[nodiscard]] auto get_ref(const UUID& id) const -> const TilesetRef&;

  [[nodiscard]] auto size() const -> usize;

  [[nodiscard]] auto empty() const -> bool;

  [[nodiscard]] auto active_tileset_id() const -> const Maybe<UUID>&;

  [[nodiscard]] auto next_tile_id() const noexcept -> TileID { return mNextTileId; }

  [[nodiscard]] auto begin() const noexcept { return mRefs.begin(); }
  [[nodiscard]] auto end() const noexcept { return mRefs.end(); }

 private:
  HashMap<UUID, TilesetRef> mRefs;
  HashMap<TileID, UUID>     mTileCache;
  Maybe<UUID>               mActiveTileset;
  TileID                    mNextTileId{1};
};

}  // namespace tactile::core
