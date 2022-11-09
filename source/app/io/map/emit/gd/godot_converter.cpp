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

#include "godot_converter.hpp"

#include <algorithm>  // replace
#include <cmath>      // sin, cos
#include <numbers>    // pi
#include <utility>    // move

#include <fmt/format.h>

#include "core/tile_pos.hpp"
#include "core/type/string.hpp"
#include "core/type/vec.hpp"
#include "core/util/assoc.hpp"
#include "core/util/functional.hpp"
#include "io/map/emit/gd/godot_options.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

using TilesetTextures = HashMap<UUID, GdExtRes>;

[[nodiscard]] auto to_godot_name(StringView name) -> String
{
  String copy {name};
  std::replace(copy.begin(), copy.end(), '/', '-');
  return copy;
}

[[nodiscard]] auto copy_meta(const ir::ContextData& context) -> GdMetaData
{
  GdMetaData meta;

  for (const auto& [name, value]: context.properties) {
    meta.props[name] = value;
  }

  for (const auto& [name, attrs]: context.components) {
    auto& comp = meta.comps[name];
    for (const auto& [attr_name, attr_value]: attrs) {
      comp[attr_name] = attr_value;
    }
  }

  return meta;
}

[[nodiscard]] auto join_tilesets(const ir::MapData& map, const GodotEmitOptions& options)
    -> GodotTileset
{
  GodotTileset result;

  for (const auto& tileset: map.tilesets) {
    const auto image_name = tileset.image_path.filename().string();
    const auto texture_id =
        result.add_texture(options.project_image_dir / image_name, tileset.image_path);

    GdTilesetInfo ts_info = {
        .uuid = tileset.uuid,
        .name = tileset.name,
        .texture_ref = texture_id,
        .first_tile = tileset.first_tile,
        .tile_size = tileset.tile_size,
        .image_size = tileset.image_size,
    };

    result.add_tileset(std::move(ts_info));
  }

  return result;
}

void add_tileset_textures(const ir::MapData& map,
                          const GodotEmitOptions& options,
                          GodotFile& file,
                          HashMap<UUID, GdExtRes>& tileset_textures)
{
  for (const auto& tileset: map.tilesets) {
    const auto filename = tileset.name + tileset.image_path.extension().string();
    const auto relpath = options.project_image_dir / filename;
    const auto id =
        file.add_ext_resource(fmt::format("res://{}", relpath.string()), "Texture");

    tileset_textures[tileset.uuid] = id;
  }
}

void add_animation(const ir::TilesetData& tileset,
                   const GdExtRes texture_id,
                   const TileIndex tile_index,
                   const ir::MetaTileData& tile,
                   GodotFile& file)
{
  TACTILE_ASSERT(!tile.frames.empty());

  const auto tile_id = tileset.first_tile + tile_index;

  GdAnimation animation;
  animation.name = fmt::format("Tile {}", tile_id);
  animation.frames.reserve(tile.frames.size());

  // Godot only supports a single speed for animations, so we just use the first frame
  animation.speed = 1'000.0f / static_cast<float>(tile.frames.front().duration_ms);

  for (const auto& frame: tile.frames) {
    const auto pos = TilePos::from_index(frame.tile_index, tileset.column_count);

    GdAtlasTexture texture;
    texture.atlas_id = texture_id;
    texture.region = {pos.col_to_x(tileset.tile_size.x),
                      pos.row_to_y(tileset.tile_size.y),
                      tileset.tile_size.x,
                      tileset.tile_size.y};

    animation.frames.push_back(file.add_atlas_texture(texture));
  }

  file.add_animation(std::move(animation));
}

void add_animations(const ir::MapData& map,
                    const TilesetTextures& tileset_textures,
                    GodotFile& file)
{
  for (const auto& tileset: map.tilesets) {
    for (const auto& [tile_index, tile]: tileset.fancy_tiles) {
      if (!tile.frames.empty()) {
        const auto texture_id = lookup_in(tileset_textures, tileset.uuid);
        add_animation(tileset, texture_id, tile_index, tile, file);
      }
    }
  }
}

[[nodiscard]] auto approximate_ellipse_as_polygon(const ir::ObjectData& object,
                                                  const usize point_count) -> Vec<Float2>
{
  TACTILE_ASSERT(object.type == ObjectType::Ellipse);

  Vec<Float2> points;
  points.reserve(point_count);

  const auto n = static_cast<double>(point_count);
  const auto radius = object.size / 2.0f;

  constexpr auto tau = std::numbers::pi * 2.0;
  for (usize i = 0; i < point_count; ++i) {
    const auto theta = static_cast<double>(i) / n * tau;
    const auto x = radius.x * std::cos(theta);
    const auto y = radius.y * std::sin(theta);
    points.emplace_back(x, y);
  }

  return points;
}

[[nodiscard]] auto create_object(const GodotEmitOptions& options,
                                 const ir::ObjectData& object,
                                 String parent,
                                 GodotFile& file) -> GdObject
{
  auto object_name = fmt::format("Object {}", object.id);
  if (!object.name.empty()) {
    object_name += fmt::format(" ('{}')", object.name);
  }

  GdObject gd_object;
  gd_object.name = std::move(object_name);
  gd_object.parent = std::move(parent);
  gd_object.position = object.pos + (object.size / 2.0f);
  gd_object.visible = object.visible;
  gd_object.meta = copy_meta(object.context);

  if (object.type == ObjectType::Rect) {
    GdRectShape shape;
    shape.extents = object.size / 2.0f;

    const auto shape_id = file.add_shape(shape);

    auto& rect = gd_object.value.emplace<GdRect>();
    rect.shape = shape_id;
  }
  else if (object.type == ObjectType::Ellipse) {
    auto& polygon = gd_object.value.emplace<GdPolygon>();
    polygon.points =
        approximate_ellipse_as_polygon(object, options.ellipse_polygon_point_count);
  }
  else {
    TACTILE_ASSERT(object.type == ObjectType::Point);
    gd_object.value.emplace<GdPoint>();
    gd_object.position = object.pos;
  }

  return gd_object;
}

void add_object_layer(const GodotEmitOptions& options,
                      const ir::LayerData& layer,
                      String parent,
                      GodotScene& scene)
{
  const auto& object_layer = layer.as_object_layer();

  GdLayer gd_layer = {
      .name = to_godot_name(layer.name),
      .parent = std::move(parent),
      .meta = copy_meta(layer.context),
      .visible = layer.visible,
  };

  auto& gd_object_layer = gd_layer.value.emplace<GdObjectLayer>();
  gd_object_layer.objects.reserve(object_layer.objects.size());

  const auto object_parent =
      (gd_layer.parent == ".") ? to_godot_name(layer.name)
                               : fmt::format("{}/{}", parent, to_godot_name(layer.name));

  for (const auto& object: object_layer.objects) {
    gd_object_layer.objects.push_back(
        create_object(options, object, object_parent, scene));
  }

  scene.add_layer(std::move(gd_layer));
}

void add_tile_layer(const ir::MapData& map,
                    const ir::LayerData& layer,
                    String parent,
                    GodotScene& scene)
{
  const auto& tile_layer = layer.as_tile_layer();

  GdLayer gd_layer = {
      .name = to_godot_name(layer.name),
      .parent = std::move(parent),
      .meta = copy_meta(layer.context),
      .visible = layer.visible,
  };

  auto& gd_tile_layer = gd_layer.value.emplace<GdTileLayer>();
  gd_tile_layer.cell_size = map.tile_size;
  gd_tile_layer.data.reserve(3 * map.row_count * map.col_count);

  const auto rows = tile_layer.row_count;
  const auto cols = tile_layer.col_count;
  invoke_mn(rows, cols, [&](const usize row, const usize col) {
    const auto tile_id = tile_layer.tiles[row][col];
    if (tile_id != empty_tile) {
      const auto& tileset = map.find_tileset_with_tile(tile_id);

      constexpr int32 tile_offset = 65'536;
      const auto tile_index = tile_id - tileset.first_tile;

      auto& encoded = gd_tile_layer.data.emplace_back();
      encoded.tile_index = tile_index;

      if (encoded.tile_index >= tileset.column_count) {
        const auto tileset_pos =
            TilePos::from_index(encoded.tile_index, tileset.column_count);
        encoded.tile_index = tileset_pos.col() + (tileset_pos.row() * tile_offset);
      }

      encoded.position =
          static_cast<int32>(col) + (static_cast<int32>(row) * tile_offset);

      encoded.tileset = scene.tileset().index_of(tileset.uuid) + 1;

      if (tileset.is_animated(tile_index)) {
        auto& animation = gd_tile_layer.animations.emplace_back();
        animation.tile_id = tile_id;
        animation.row = row;
        animation.col = col;
        animation.parent = fmt::format("{}/{}", gd_layer.parent, gd_layer.name);
      }
    }
  });

  scene.add_layer(std::move(gd_layer));
}

void add_layer(const GodotEmitOptions& options,
               const ir::MapData& map,
               const ir::LayerData& layer,
               String parent,
               GodotScene& scene)
{
  switch (layer.type) {
    case LayerType::TileLayer:
      add_tile_layer(map, layer, std::move(parent), scene);
      break;

    case LayerType::ObjectLayer:
      add_object_layer(options, layer, std::move(parent), scene);
      break;

    case LayerType::GroupLayer: {
      const auto layer_name = to_godot_name(layer.name);

      scene.add_layer({
          .name = layer_name,
          .parent = parent,
          .meta = copy_meta(layer.context),
          .visible = layer.visible,
      });

      const auto child_parent_path =
          (parent == ".") ? layer_name : fmt::format("{}/{}", parent, layer_name);

      const auto& group_layer = layer.as_group_layer();
      for (const auto& child_layer: group_layer.children) {
        add_layer(options, map, *child_layer, child_parent_path, scene);
      }

      break;
    }
  }
}

}  // namespace

// TODO sibling nodes cannot have the same names, validate!
auto convert_to_godot(const ir::MapData& map, const GodotEmitOptions& options)
    -> GodotScene
{
  GodotScene scene;
  scene.set_tileset(join_tilesets(map, options),
                    options.project_tileset_dir / "tileset.tres");
  scene.set_root_meta(copy_meta(map.context));

  // Temporary cache that maps tileset UUIDs to the associated external resource IDs
  TilesetTextures tileset_textures;

  add_tileset_textures(map, options, scene, tileset_textures);
  add_animations(map, tileset_textures, scene);

  for (const auto& layer: map.layers) {
    add_layer(options, map, layer, ".", scene);
  }

  return scene;
}

}  // namespace tactile::io
