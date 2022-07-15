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

#include "core/common/identifiers.hpp"
#include "core/common/maybe.hpp"
#include "core/common/memory.hpp"
#include "core/common/uuid.hpp"
#include "core/region.hpp"
#include "core/tileset/tileset.hpp"
#include "core/viewport.hpp"

namespace tactile {

// TODO improve selection

class TilesetRef final
{
 public:
  TilesetRef(Shared<Tileset> tileset, TileID first, TileID last);

  void update();

  void set_selection(const Maybe<Region>& selection);

  void set_embedded(bool embedded);

  /// Converts a tile identifier to the corresponding tile index.
  [[nodiscard]] auto to_index(TileID id) const -> TileIndex;

  [[nodiscard]] auto is_valid_tile(TileID id) const -> bool;

  [[nodiscard]] auto is_single_tile_selected() const -> bool;

  [[nodiscard]] auto get_selection() const -> const Maybe<Region>&;

  [[nodiscard]] auto get_tileset() -> const Shared<Tileset>&;

  [[nodiscard]] auto view_tileset() const -> const Tileset&;

  [[nodiscard]] auto first_tile() const noexcept -> TileID { return mFirstTile; }
  [[nodiscard]] auto last_tile() const noexcept -> TileID { return mLastTile; }

  [[nodiscard]] auto get_viewport() -> Viewport& { return mViewport; }
  [[nodiscard]] auto get_viewport() const -> const Viewport& { return mViewport; }

  [[nodiscard]] auto is_embedded() const noexcept -> bool { return mEmbedded; }

 private:
  Shared<Tileset> mTileset;          /// The referenced tileset.
  TileID          mFirstTile {};     /// The first associated tile ID (inclusive).
  TileID          mLastTile {};      /// The last associated tile ID (inclusive).
  Maybe<Region>   mSelection;        /// Selected region.
  Viewport        mViewport;         /// Viewport in tileset dock.
  bool            mEmbedded : 1 {};  /// Whether the tileset is embedded in the map.
};

}  // namespace tactile
