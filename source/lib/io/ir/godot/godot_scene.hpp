// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "io/ir/godot/godot_file.hpp"
#include "io/ir/godot/godot_tileset.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/base/container/string.hpp"
#include "tactile/base/container/variant.hpp"
#include "tactile/base/container/vector.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/numeric/vec.hpp"

namespace tactile {

struct GdPoint final {};

struct GdRect final {
  GdSubRes shape {};
};

struct GdPolygon final {
  Vector<Float2> points;
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
  Vector<GdObject> objects;
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
  Vector<GdEncodedTile> data;
  Vector<GdTileAnimation> animations;
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

  [[nodiscard]] auto layers() const -> const Vector<GdLayer>&;

 private:
  GdExtRes mTilesetId {};
  GodotTileset mTileset;
  Vector<GdLayer> mLayers;
};

}  // namespace tactile
