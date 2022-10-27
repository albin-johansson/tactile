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

#include "core/ctx/context.hpp"
#include "core/ctx/context_info.hpp"
#include "core/tile/tileset_info.hpp"
#include "core/tile_pos.hpp"
#include "core/type/hash_map.hpp"
#include "core/type/math.hpp"
#include "core/type/path.hpp"
#include "core/type/ptr.hpp"
#include "core/uuid.hpp"
#include "core/vocabulary.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Tile);

/// Represents a collection of tiles encoded in a texture.
class Tileset final : public Context {
  using Tiles = HashMap<UUID, Shared<Tile>>;
  using iterator = Tiles::iterator;

 public:
  explicit Tileset(TilesetInfo info, const UUID& id = make_uuid());

  ~Tileset() noexcept override;

  void accept(ContextVisitor& visitor) const override;

  /// Updates animations and internal caches.
  void update();

  /// Returns the tile at a specific index.
  [[nodiscard]] auto operator[](TileIndex index) -> Tile&;
  [[nodiscard]] auto operator[](TileIndex index) const -> const Tile&;

  /// Returns a shared pointer to the tile at a specific index.
  /// Prefer use of the subscript operator if you only need to view the tile.
  [[nodiscard]] auto get_tile_ptr(TileIndex index) -> const Shared<Tile>&;

  /// Converts a (valid) tile position to a tile index in the tile set.
  [[nodiscard]] auto index_of(const TilePos& pos) const -> TileIndex;

  /// Returns the index of the tile that should be rendered for a specific tile.
  [[nodiscard]] auto appearance_of(TileIndex index) const -> TileIndex;

  /// Indicates whether the tile set has a tile at a specific position.
  [[nodiscard]] auto is_valid(const TilePos& pos) const -> bool;

  [[nodiscard]] auto ctx() -> ContextInfo& override;
  [[nodiscard]] auto ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto uuid() const -> const UUID& override;

  /// Returns the associated texture.
  [[nodiscard]] auto texture() const -> const Texture&;

  /// Returns the identifier for the associated texture.
  [[nodiscard, deprecated]] auto texture_id() const noexcept -> uint;

  /// Returns the source path of the associated texture.
  [[nodiscard, deprecated]] auto texture_path() const -> const Path&;

  /// Returns the size of the associated texture.
  [[nodiscard, deprecated]] auto texture_size() const noexcept -> const Int2&;

  /// Returns the size of each tile.
  [[nodiscard]] auto tile_size() const noexcept -> const Int2&;

  /// Returns the normalized size of a tile in the tile set.
  [[nodiscard]] auto uv_size() const noexcept -> const Float2&;

  /// Returns the amount of tile rows.
  [[nodiscard]] auto row_count() const noexcept -> int32;

  /// Returns the amount of tile columns.
  [[nodiscard]] auto column_count() const noexcept -> int32;

  /// Returns the total amount of tiles.
  [[nodiscard]] auto tile_count() const noexcept -> int32;

  [[nodiscard]] auto begin() const noexcept -> iterator;
  [[nodiscard]] auto end() const noexcept -> iterator;

 private:
  struct Data;
  Unique<Data> mData;

  void load_tiles();
};

}  // namespace tactile
