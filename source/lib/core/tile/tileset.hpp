// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/tile/tileset_info.hpp"
#include "tactile/base/container/hash_map.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"
#include "tactile/base/prelude.hpp"
#include "tactile/core/numeric/vec.hpp"
#include "tactile/core/util/uuid.hpp"
#include "tile_pos.hpp"

namespace tactile {

TACTILE_FWD_DECLARE_CLASS(Tile);

/// Represents a collection of tiles encoded in a texture.
class Tileset final : public Context {
  using Tiles = HashMap<UUID, Shared<Tile>>;
  using iterator = Tiles::iterator;

 public:
  explicit Tileset(TilesetInfo info, const UUID& id = UUID::generate());

  ~Tileset() noexcept override;

  void accept(ContextVisitor& visitor) const override;

  /// Updates animations and internal caches.
  void update();

  /// Returns the tile at a specific index.
  [[nodiscard]] auto operator[](TileIndex index) -> Tile&;
  [[nodiscard]] auto operator[](TileIndex index) const -> const Tile&;

  /// Marks a tile, denoted by index, as selected.
  void select_tile(TileIndex index);

  /// Returns the currently selected tile, if there is one.
  [[nodiscard]] auto get_selected_tile() const -> Maybe<TileIndex>;

  /// Returns a shared pointer to the tile at a specific index.
  /// Prefer use of the subscript operator if you only need to view the tile.
  [[nodiscard]] auto get_tile_ptr(TileIndex index) -> const Shared<Tile>&;

  /// Converts a (valid) tile position to a tile index in the tile set.
  [[nodiscard]] auto index_of(const TilePos& pos) const -> TileIndex;

  /// Returns the index of the tile that should be rendered for a specific tile.
  [[nodiscard]] auto appearance_of(TileIndex index) const -> TileIndex;

  /// Indicates whether the tile set has a tile at a specific position.
  [[nodiscard]] auto is_valid(const TilePos& pos) const -> bool;

  [[nodiscard]] auto get_ctx() -> ContextInfo& override;
  [[nodiscard]] auto get_ctx() const -> const ContextInfo& override;

  [[nodiscard]] auto get_uuid() const -> const UUID& override;

  /// Returns the associated texture.
  [[nodiscard]] auto texture() const -> const Texture&;

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
