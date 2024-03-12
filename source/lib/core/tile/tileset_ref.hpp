// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "core/region.hpp"
#include "core/tile/tileset.hpp"
#include "core/viewport.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/smart_ptr.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/util/uuid.hpp"

namespace tactile {

// TODO improve selection

class TilesetRef final {
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

  [[nodiscard]] auto get_tileset_ptr() -> const Shared<Tileset>&;

  [[nodiscard]] auto get_tileset() const -> const Tileset&;

  [[nodiscard]] auto get_first_tile() const noexcept -> TileID { return mFirstTile; }
  [[nodiscard]] auto get_last_tile() const noexcept -> TileID { return mLastTile; }

  [[nodiscard]] auto get_viewport() -> Viewport& { return mViewport; }
  [[nodiscard]] auto get_viewport() const -> const Viewport& { return mViewport; }

  [[nodiscard]] auto is_embedded() const noexcept -> bool { return mEmbedded; }

 private:
  Shared<Tileset> mTileset;  /// The referenced tileset.
  TileID mFirstTile {};      /// The first associated tile ID (inclusive).
  TileID mLastTile {};       /// The last associated tile ID (inclusive).
  Maybe<Region> mSelection;  /// Selected region.
  Viewport mViewport;        /// Viewport in tileset dock.
  bool mEmbedded : 1 {};     /// Whether the tileset is embedded in the map.
};

}  // namespace tactile
