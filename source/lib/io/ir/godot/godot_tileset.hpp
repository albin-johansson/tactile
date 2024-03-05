/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "common/type/math.hpp"
#include "common/type/pair.hpp"
#include "common/type/uuid.hpp"
#include "common/type/vec.hpp"
#include "common/util/assoc.hpp"
#include "io/ir/godot/godot_file.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/id.hpp"
#include "tactile/base/int.hpp"

namespace tactile {

/// Information about a tileset in the merged tileset collection.
struct GdTilesetInfo final {
  UUID uuid;  /// Identifier of associated IR-tileset.

  String name;
  GdExtRes texture_ref {};
  TileID first_tile {};
  Int2 tile_size {};
  Int2 image_size {};
};

/// Intermediate representation of an amalgamated Godot tileset, stored in its own file.
class GodotTileset final : public GodotFile {
 public:
  using TextureNamePair = Pair<Path, String>;

  auto add_texture(const Path& dest, Path source) -> GdExtRes;

  void add_tileset(GdTilesetInfo info);

  [[nodiscard]] auto index_of(const UUID& tileset_id) const -> int32;

  [[nodiscard]] auto texture_paths() const -> const Vec<TextureNamePair>&;

  [[nodiscard]] auto tilesets() const -> const Vec<GdTilesetInfo>&;

 private:
  Vec<TextureNamePair> mSourceTexturePaths;
  Vec<GdTilesetInfo> mTilesetInfos;
};

}  // namespace tactile
