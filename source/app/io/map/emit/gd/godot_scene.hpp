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

#include <string>   // string
#include <variant>  // variant, monostate
#include <vector>   // vector

#include "core/common/fs.hpp"
#include "core/common/math.hpp"
#include "core/common/vocabulary.hpp"
#include "io/map/emit/gd/godot_file.hpp"
#include "io/map/emit/gd/godot_tileset.hpp"

namespace tactile::io {

struct GdPoint final {};

struct GdRect final {
  GdSubRes shape {};
};

struct GdPolygon final {
  std::vector<Float2> points;
};

struct GdObject final {
  using Value = std::variant<GdRect, GdPolygon, GdPoint>;

  std::string name;
  std::string parent;
  Float2 position {};
  Value value;
  GdMetaData meta;
  bool visible : 1 {};
};

struct GdObjectLayer final {
  std::vector<GdObject> objects;
};

struct GdEncodedTile final {
  int32 position {};
  int32 tileset {};
  int32 tile_index {};
};

struct GdTileAnimation final {
  std::string parent;
  usize row {};
  usize col {};
  TileID tile_id {};
};

struct GdTileLayer final {
  Int2 cell_size {};
  std::vector<GdEncodedTile> data;
  std::vector<GdTileAnimation> animations;
};

struct GdLayer final {
  using Value = std::variant<std::monostate, GdTileLayer, GdObjectLayer>;

  std::string name;
  std::string parent;
  Value value;
  GdMetaData meta;
  bool visible : 1 {};
};

/// This class serves as an intermediate representation of a Godot scene.
/// This is used by the Godot emitter to simplify the emission of Godot maps.
class GodotScene final : public GodotFile {
 public:
  void set_tileset(GodotTileset tileset, const fs::path& dest);

  void add_layer(GdLayer layer);

  [[nodiscard]] auto tileset_id() const -> GdExtRes;
  [[nodiscard]] auto tileset() const -> const GodotTileset&;

  [[nodiscard]] auto layers() const -> const std::vector<GdLayer>&;

 private:
  GdExtRes mTilesetId {};
  GodotTileset mTileset;
  std::vector<GdLayer> mLayers;
};

}  // namespace tactile::io