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

#include "common/numeric.hpp"
#include "common/type/math.hpp"
#include "common/type/path.hpp"
#include "common/type/string.hpp"
#include "common/type/variant.hpp"
#include "common/type/vec.hpp"
#include "io/map/emit/gd/godot_file.hpp"
#include "io/map/emit/gd/godot_tileset.hpp"

namespace tactile {

struct GdPoint final {};

struct GdRect final {
  GdSubRes shape {};
};

struct GdPolygon final {
  Vec<Float2> points;
};

struct GdObject final {
  using Value = Variant<GdRect, GdPolygon, GdPoint>;

  String name;
  String parent;
  Float2 position {};
  Value value;
  GdMetaData meta;
  bool visible : 1 {};
};

struct GdObjectLayer final {
  Vec<GdObject> objects;
};

struct GdEncodedTile final {
  int32 position {};
  int32 tileset {};
  int32 tile_index {};
};

struct GdTileAnimation final {
  String parent;
  usize row {};
  usize col {};
  TileID tile_id {};
};

struct GdTileLayer final {
  Int2 cell_size {};
  Vec<GdEncodedTile> data;
  Vec<GdTileAnimation> animations;
};

struct GdLayer final {
  using Value = Variant<Monostate, GdTileLayer, GdObjectLayer>;

  String name;
  String parent;
  Value value;
  GdMetaData meta;
  bool visible : 1 {};
};

/// This class serves as an intermediate representation of a Godot scene.
/// This is used by the Godot emitter to simplify the emission of Godot maps.
class GodotScene final : public GodotFile {
 public:
  void set_tileset(GodotTileset tileset, const Path& dest);

  void add_layer(GdLayer layer);

  [[nodiscard]] auto tileset_id() const -> GdExtRes;
  [[nodiscard]] auto tileset() const -> const GodotTileset&;

  [[nodiscard]] auto layers() const -> const Vec<GdLayer>&;

 private:
  GdExtRes mTilesetId {};
  GodotTileset mTileset;
  Vec<GdLayer> mLayers;
};

}  // namespace tactile
