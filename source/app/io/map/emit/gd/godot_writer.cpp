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

// Reference: https://docs.godotengine.org/en/stable/development/file_formats/tscn.html

#include "godot_writer.hpp"

#include <variant>  // get, holds_alternative

#include "core/tile_pos.hpp"
#include "core/type/ostream.hpp"
#include "core/util/filesystem.hpp"
#include "core/util/fmt.hpp"
#include "core/util/str.hpp"
#include "core/vocabulary.hpp"
#include "io/file.hpp"
#include "io/map/emit/gd/godot_options.hpp"
#include "io/map/emit/gd/godot_scene.hpp"
#include "misc/assert.hpp"

namespace tactile::io {
namespace {

[[nodiscard]] auto write_attribute(StringView name, const Attribute& value) -> String
{
  switch (value.type()) {
    case AttributeType::String:
      return format_str(R"("{}": "{}")", name, value.as_string());

    case AttributeType::Int:
      return format_str(R"("{}": {})", name, value.as_int());

    case AttributeType::Float:
      return format_str(R"("{}": {})", name, value.as_float());

    case AttributeType::Bool:
      return format_str(R"("{}": {})", name, value.as_bool() ? "true" : "false");

    case AttributeType::Path:
      return format_str(R"("{}": "{}")",
                        name,
                        convert_to_forward_slashes(value.as_path()));

    case AttributeType::Color: {
      const auto& color = value.as_color();
      return format_str(R"("{}": Color( {}, {}, {}, {} ))",
                        name,
                        color.norm_red(),
                        color.norm_green(),
                        color.norm_blue(),
                        color.norm_alpha());
    }
    case AttributeType::Object:
      return format_str(R"("{}": {})", name, static_cast<int32>(value.as_object()));

    default:
      throw TactileError {"Invalid attribute type!"};
  }
}

void write_attributes(OStream& stream, const GdAttributes& attrs, StringView prefix)
{
  const auto count = attrs.size();

  usize index = 0;
  for (const auto& [name, value]: attrs) {
    stream << to_std_view(prefix) << write_attribute(name, value);

    if (index < count - 1) {
      stream << ',';
    }

    stream << '\n';
    ++index;
  }
}

void write_components(OStream& stream, const GdMetaData& meta)
{
  TACTILE_ASSERT(!meta.comps.empty());
  stream << "  \"components\": {\n";

  bool first_comp = true;
  for (const auto& [comp_name, comp_attrs]: meta.comps) {
    if (!first_comp) {
      stream << ",\n";
    }

    stream << format_str("    \"{}\": {{\n", comp_name);
    write_attributes(stream, comp_attrs, "      ");
    stream << "    }";

    first_comp = false;
  }

  stream << "\n  }\n";
}

void write_properties(OStream& stream, const GdMetaData& meta)
{
  TACTILE_ASSERT(!meta.props.empty());
  stream << "  \"properties\": {\n";

  write_attributes(stream, meta.props, "    ");

  stream << "  }";
}

void write_metadata(OStream& stream, const GdMetaData& meta)
{
  const auto has_props = !meta.props.empty();
  const auto has_comps = !meta.comps.empty();

  if (!has_props && !has_comps) {
    return;
  }

  stream << "__meta__ = {\n";

  if (has_props) {
    write_properties(stream, meta);
    if (has_comps) {
      stream << ',';
    }
    stream << '\n';
  }

  if (has_comps) {
    write_components(stream, meta);
  }

  stream << "}\n";
}

void write_ext_resources(OStream& stream, const GodotFile::ExtResources& resources)
{
  if (!resources.empty()) {
    stream << '\n';
    for (const auto& [id, resource]: resources) {
      stream << format_str("[ext_resource path=\"{}\" type=\"{}\" id={}]\n",
                           resource.path,
                           resource.type,
                           id);
    }
  }
}

void write_tileset_file(const GodotTileset& tileset, const GodotEmitOptions& options)
{
  const auto path = options.root_dir / options.project_tileset_dir / "tileset.tres";
  auto stream = write_file(path, FileType::Text);

  stream << format_str("[gd_resource type=\"TileSet\" load_steps={} format=2]\n",
                       tileset.get_load_steps());

  write_ext_resources(stream, tileset.ext_resources());

  for (const auto& [source, filename]: tileset.texture_paths()) {
    const auto relpath = options.project_image_dir / to_std_view(filename);
    const auto dest = options.root_dir / relpath;
    fs::copy(source, dest, fs::copy_options::overwrite_existing);
  }

  stream << "\n[resource]\n";

  for (usize index = 0; const auto& info: tileset.tilesets()) {
    const auto prefix = format_str("{}/", index + 1);

    stream << prefix << format_str("name = \"{}\"\n", info.name);
    stream << prefix << format_str("texture = ExtResource( {} )\n", info.texture_ref);
    stream << prefix << "tex_offset = Vector2( 0, 0 )\n";
    stream << prefix << "modulate = Color( 1, 1, 1, 1 )\n";
    stream << prefix
           << format_str("region = Rect2( 0, 0, {}, {} )\n",
                         info.image_size.x,
                         info.image_size.y);
    stream << prefix << "tile_mode = 2\n";
    stream << prefix << "autotile/icon_coordinate = Vector2( 0, 0 )\n";
    stream << prefix
           << format_str("autotile/tile_size = Vector2( {}, {} )\n",
                         info.tile_size.x,
                         info.tile_size.y);
    stream << prefix << "autotile/spacing = 0\n";
    stream << prefix << "autotile/occluder_map = [  ]\n";
    stream << prefix << "autotile/navpoly_map = [  ]\n";
    stream << prefix << "autotile/priority_map = [  ]\n";
    stream << prefix << "autotile/z_index_map = [  ]\n";
    stream << prefix << "occluder_offset = Vector2( 0, 0 )\n";
    stream << prefix << "navigation_offset = Vector2( 0, 0 )\n";
    stream << prefix << "shape_offset = Vector2( 0, 0 )\n";
    stream << prefix << "shape_transform = Transform2D( 1, 0, 0, 1, 0, 0 )\n";
    stream << prefix << "shape_one_way = false\n";
    stream << prefix << "shape_one_way_margin = 0.0\n";
    stream << prefix << "shapes = [  ]\n";
    stream << prefix << "z_index = 0\n";

    ++index;
  }
}

void write_atlas_textures(OStream& stream, const GodotScene& scene)
{
  if (!scene.atlas_textures().empty()) {
    for (const auto& [id, texture]: scene.atlas_textures()) {
      stream << '\n';
      stream << format_str("[sub_resource type=\"AtlasTexture\" id={}]\n", id);
      stream << format_str("atlas = ExtResource( {} )\n", texture.atlas_id);
      stream << format_str("region = Rect2( {}, {}, {}, {} )\n",
                           texture.region.x,
                           texture.region.y,
                           texture.region.z,
                           texture.region.w);
    }
  }
}

void write_sprite_frames(OStream& stream, const GdSpriteFrames& sprite_frames)
{
  stream << format_str("\n[sub_resource type=\"SpriteFrames\" id={}]\n",
                       sprite_frames.id);
  stream << "animations = [\n";

  for (const auto& animation: sprite_frames.animations) {
    stream << "  {\n";

    stream << "    \"frames\": [ ";
    bool first_frame = true;
    for (const auto& frame: animation.frames) {
      if (!first_frame) {
        stream << ", ";
      }
      stream << format_str("SubResource( {} )", frame);
      first_frame = false;
    }
    stream << " ],\n";

    stream << "    \"loop\": true,\n";
    stream << format_str("    \"name\": \"{}\",\n", animation.name);
    stream << format_str("    \"speed\": \"{:.3f}\"\n", animation.speed);

    stream << "  }\n";
  }
  stream << "]\n";
}

void write_shapes(OStream& stream, const GodotScene& scene)
{
  for (const auto& [id, shape]: scene.rectangle_shapes()) {
    stream << format_str("\n[sub_resource type=\"RectangleShape2D\" id={}]\n", id);
    stream << format_str("extents = Vector2( {}, {} )\n",
                         shape.extents.x,
                         shape.extents.y);
  }
}

void write_tile_layer_animation_nodes(OStream& stream,
                                      const GodotScene& scene,
                                      const GdTileLayer& tile_layer)
{
  const auto& sprite_frames = scene.sprite_frames();

  for (const auto& animation: tile_layer.animations) {
    const auto name = format_str("Tile ({}, {})", animation.row, animation.col);
    stream << format_str("\n[node name=\"{}\" type=\"AnimatedSprite\" parent=\"{}\"]\n",
                         name,
                         animation.parent);

    const auto pos = TilePos::from(animation.row, animation.col);
    const auto x = pos.col_to_x(tile_layer.cell_size.x);
    const auto y = pos.row_to_y(tile_layer.cell_size.y);

    stream << format_str("position = Vector2( {}, {} )\n", x, y);
    stream << format_str("frames = SubResource( {} )\n", sprite_frames.id);
    stream << "speed_scale = 1.0\n";
    stream << format_str("animation = \"Tile {}\"\n", animation.tile_id);
    stream << "playing = true\n";
    stream << "centered = false\n";
  }
}

void write_tile_layer(OStream& stream, const GodotScene& scene, const GdLayer& layer)
{
  const auto& tile_layer = std::get<GdTileLayer>(layer.value);

  stream << format_str("\n[node name=\"{}\" type=\"TileMap\" parent=\"{}\"]\n",
                       layer.name,
                       layer.parent);
  stream << format_str("tile_set = ExtResource( {} )\n", scene.tileset_id());
  stream << "format = 1\n";
  if (!layer.visible) {
    stream << "visible = false\n";
  }

  stream << "tile_data = PoolIntArray( ";

  bool first_tile = true;
  for (const auto& encoded_tile: tile_layer.data) {
    if (!first_tile) {
      stream << ", ";
    }

    stream << format_str("{}, {}, {}",
                         encoded_tile.position,
                         encoded_tile.tileset,
                         encoded_tile.tile_index);

    first_tile = false;
  }

  stream << " )\n";
  stream << format_str("cell_size = Vector2( {}, {} )\n",
                       tile_layer.cell_size.x,
                       tile_layer.cell_size.y);

  write_metadata(stream, layer.meta);

  write_tile_layer_animation_nodes(stream, scene, tile_layer);
}

void write_rectangle_object(OStream& stream, const GdObject& object)
{
  const auto& rect = std::get<GdRect>(object.value);

  stream << format_str("\n[node name=\"{}\" type=\"Area2D\" parent=\"{}\"]\n",
                       object.name,
                       object.parent);
  stream << format_str("position = Vector2( {:.3f}, {:.3f} )\n",
                       object.position.x,
                       object.position.y);
  if (!object.visible) {
    stream << "visible = false\n";
  }

  write_metadata(stream, object.meta);

  stream << format_str(
      "\n[node name=\"Shape\" type=\"CollisionShape2D\" parent=\"{}/{}\"]\n",
      object.parent,
      object.name);
  stream << format_str("shape = SubResource( {} )\n", rect.shape);
}

void write_polygon_object(OStream& stream, const GdObject& object)
{
  const auto& polygon = std::get<GdPolygon>(object.value);

  stream << format_str("\n[node name=\"{}\" type=\"Area2D\" parent=\"{}\"]\n",
                       object.name,
                       object.parent);
  stream << format_str("position = Vector2( {:.3f}, {:.3f} )\n",
                       object.position.x,
                       object.position.y);
  if (!object.visible) {
    stream << "visible = false\n";
  }

  write_metadata(stream, object.meta);

  stream << format_str(
      "\n[node name=\"Shape\" type=\"CollisionPolygon2D\" parent=\"{}/{}\"]\n",
      object.parent,
      object.name);

  stream << "polygon = PoolVector2Array( ";

  bool first_point = true;
  for (const auto& point: polygon.points) {
    if (!first_point) {
      stream << ", ";
    }
    stream << format_str("{:.3f}, {:.3f}", point.x, point.y);
    first_point = false;
  }

  stream << " )\n";
}

void write_object(OStream& stream, const GdObject& object)
{
  if (std::holds_alternative<GdRect>(object.value)) {
    write_rectangle_object(stream, object);
  }
  else if (std::holds_alternative<GdPolygon>(object.value)) {
    write_polygon_object(stream, object);
  }
  else {
    stream << format_str("\n[node name=\"{}\" type=\"Node2D\" parent=\"{}\"]\n",
                         object.name,
                         object.parent);
    stream << format_str("position = Vector2( {:.3f}, {:.3f} )\n",
                         object.position.x,
                         object.position.y);
    if (!object.visible) {
      stream << "visible = false\n";
    }

    write_metadata(stream, object.meta);
  }
}

void write_object_layer(OStream& stream, const GdLayer& layer)
{
  const auto& object_layer = std::get<GdObjectLayer>(layer.value);

  stream << format_str("\n[node name=\"{}\" type=\"Node2D\" parent=\"{}\"]\n",
                       layer.name,
                       layer.parent);
  if (!layer.visible) {
    stream << "visible = false\n";
  }

  write_metadata(stream, layer.meta);

  for (const auto& object: object_layer.objects) {
    write_object(stream, object);
  }
}

void write_layers(OStream& stream, const GodotScene& scene)
{
  for (const auto& layer: scene.layers()) {
    if (std::holds_alternative<GdTileLayer>(layer.value)) {
      write_tile_layer(stream, scene, layer);
    }
    else if (std::holds_alternative<GdObjectLayer>(layer.value)) {
      write_object_layer(stream, layer);
    }
    else {
      stream << format_str("\n[node name=\"{}\" type=\"Node2D\" parent=\"{}\"]\n",
                           layer.name,
                           layer.parent);
      if (!layer.visible) {
        stream << "visible = false\n";
      }
    }
  }
}

}  // namespace

void write_godot_scene(const GodotScene& scene, const GodotEmitOptions& options)
{
  const auto& tileset = scene.tileset();
  write_tileset_file(tileset, options);

  const auto path = options.root_dir / options.project_map_dir / "map.tscn";
  auto stream = write_file(path, FileType::Text);

  stream << format_str("[gd_scene load_steps={} format=2]\n", scene.get_load_steps());

  write_ext_resources(stream, scene.ext_resources());
  write_atlas_textures(stream, scene);

  const auto& sprite_frames = scene.sprite_frames();
  if (!sprite_frames.animations.empty()) {
    write_sprite_frames(stream, sprite_frames);
  }

  write_shapes(stream, scene);

  stream << "\n[node name=\"Root\" type=\"Node2D\"]\n";
  write_metadata(stream, scene.root_meta());

  write_layers(stream, scene);
}

}  // namespace tactile::io