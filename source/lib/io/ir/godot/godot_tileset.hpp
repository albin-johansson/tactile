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

#include "common/type/uuid.hpp"
#include "common/util/assoc.hpp"
#include "io/ir/godot/godot_file.hpp"
#include "tactile/core/io/filesystem.hpp"
#include "tactile/core/math/vector.hpp"
#include "tactile/core/prelude.hpp"
#include "tactile/core/type/string.hpp"
#include "tactile/core/type/vector.hpp"

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

struct GodotTexturePath final {
  Path path;
  String name;
};

/// Intermediate representation of an amalgamated Godot tileset, stored in its own file.
class GodotTileset final : public GodotFile {
 public:
  auto add_texture(const Path& dest, Path source) -> GdExtRes;

  void add_tileset(GdTilesetInfo info);

  [[nodiscard]] auto index_of(const UUID& tileset_id) const -> int32;

  [[nodiscard]] auto texture_paths() const -> const Vector<GodotTexturePath>&;

  [[nodiscard]] auto tilesets() const -> const Vector<GdTilesetInfo>&;

 private:
  Vector<GodotTexturePath> mSourceTexturePaths;
  Vector<GdTilesetInfo> mTilesetInfos;
};

}  // namespace tactile
