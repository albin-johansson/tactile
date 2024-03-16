// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

// Reference: https://docs.godotengine.org/en/stable/development/file_formats/tscn.html

#include "godot_writer.hpp"

#include <ostream>
#include <variant>  // get, holds_alternative

#include <fmt/format.h>

#include "common/util/filesystem.hpp"
#include "core/tile/tile_pos.hpp"
#include "io/ir/godot/godot_options.hpp"
#include "io/ir/godot/godot_scene.hpp"
#include "io/stream.hpp"
#include "tactile/base/int.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {
namespace {

[[nodiscard]] auto write_attribute(StringView name, const Attribute& value) -> String
{
  switch (value.get_type()) {
    case AttributeType::kStr:
      return fmt::format(R"("{}": "{}")", name, value.as_string());

    case AttributeType::kInt: return fmt::format(R"("{}": {})", name, value.as_int());

    case AttributeType::kInt2: {
      const auto& vec = value.as_int2();
      return fmt::format(R"("{}": Vector2( {}, {} ))", name, vec.x(), vec.y());
    }
    case AttributeType::kInt3: {
      const auto& vec = value.as_int3();
      return fmt::format(R"("{}": Vector3( {}, {}, {} ))",
                         name,
                         vec.x(),
                         vec.y(),
                         vec.z());
    }
    case AttributeType::kInt4: {
      const auto& vec = value.as_int4();
      return fmt::format(R"("{}": [ {}, {}, {}, {} ])",
                         name,
                         vec.x(),
                         vec.y(),
                         vec.z(),
                         vec.w());
    }
    case AttributeType::kFloat: return fmt::format(R"("{}": {})", name, value.as_float());

    case AttributeType::kFloat2: {
      const auto& vec = value.as_float2();
      return fmt::format(R"("{}": Vector2( {}, {} ))", name, vec.x(), vec.y());
    }
    case AttributeType::kFloat3: {
      const auto& vec = value.as_float3();
      return fmt::format(R"("{}": Vector3( {}, {}, {} ))",
                         name,
                         vec.x(),
                         vec.y(),
                         vec.z());
    }
    case AttributeType::kFloat4: {
      const auto& vec = value.as_float4();
      return fmt::format(R"("{}": [ {}, {}, {}, {} ]))",
                         name,
                         vec.x(),
                         vec.y(),
                         vec.z(),
                         vec.w());
    }
    case AttributeType::kBool:
      return fmt::format(R"("{}": {})", name, value.as_bool() ? "true" : "false");

    case AttributeType::kPath:
      return fmt::format(R"("{}": "{}")", name, use_forward_slashes(value.as_path()));

    case AttributeType::kColor: {
      const auto color = value.as_color().normalized();
      return fmt::format(R"("{}": Color( {}, {}, {}, {} ))",
                         name,
                         color[0],
                         color[1],
                         color[2],
                         color[3]);
    }
    case AttributeType::kObject:
      return fmt::format(R"("{}": {})", name, value.as_object().value);

    default: throw Exception {"Invalid attribute type"};
  }
}

void write_attributes(std::ostream& stream, const GdAttributes& attrs, StringView prefix)
{
  const auto count = attrs.size();

  usize index = 0;
  for (const auto& [name, value] : attrs) {
    stream << prefix << write_attribute(name, value);

    if (index < count - 1) {
      stream << ',';
    }

    stream << '\n';
    ++index;
  }
}

void write_components(std::ostream& stream, const GdMetaData& meta)
{
  TACTILE_ASSERT(!meta.comps.empty());
  stream << "  \"components\": {\n";

  bool first_comp = true;
  for (const auto& [comp_name, comp_attrs] : meta.comps) {
    if (!first_comp) {
      stream << ",\n";
    }

    stream << fmt::format("    \"{}\": {{\n", comp_name);
    write_attributes(stream, comp_attrs, "      ");
    stream << "    }";

    first_comp = false;
  }

  stream << "\n  }\n";
}

void write_properties(std::ostream& stream, const GdMetaData& meta)
{
  TACTILE_ASSERT(!meta.props.empty());
  stream << "  \"properties\": {\n";

  write_attributes(stream, meta.props, "    ");

  stream << "  }";
}

void write_metadata(std::ostream& stream, const GdMetaData& meta)
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

void write_ext_resources(std::ostream& stream, const GodotFile::ExtResources& resources)
{
  if (!resources.empty()) {
    stream << '\n';
    for (const auto& [id, resource] : resources) {
      stream << fmt::format("[ext_resource path=\"{}\" type=\"{}\" id={}]\n",
                            resource.path,
                            resource.type,
                            id);
    }
  }
}

void write_tileset_file(const GodotTileset& tileset, const GodotEmitOptions& options)
{
  const auto path = options.root_dir / options.project_tileset_dir / "tileset.tres";
  auto stream = open_output_stream(path, FileType::Text).value();

  stream << fmt::format("[gd_resource type=\"TileSet\" load_steps={} format=2]\n",
                        tileset.get_load_steps());

  write_ext_resources(stream, tileset.ext_resources());

  for (const auto& [source, filename] : tileset.texture_paths()) {
    const auto relpath = options.project_image_dir / filename;
    const auto dest = options.root_dir / relpath;
    fs::copy(source, dest, fs::copy_options::overwrite_existing);
  }

  stream << "\n[resource]\n";

  for (usize index = 0; const auto& info : tileset.tilesets()) {
    const auto prefix = fmt::format("{}/", index + 1);

    stream << prefix << fmt::format("name = \"{}\"\n", info.name);
    stream << prefix << fmt::format("texture = ExtResource( {} )\n", info.texture_ref);
    stream << prefix << "tex_offset = Vector2( 0, 0 )\n";
    stream << prefix << "modulate = Color( 1, 1, 1, 1 )\n";
    stream << prefix
           << fmt::format("region = Rect2( 0, 0, {}, {} )\n",
                          info.image_size.x(),
                          info.image_size.y());
    stream << prefix << "tile_mode = 2\n";
    stream << prefix << "autotile/icon_coordinate = Vector2( 0, 0 )\n";
    stream << prefix
           << fmt::format("autotile/tile_size = Vector2( {}, {} )\n",
                          info.tile_size.x(),
                          info.tile_size.y());
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

void write_atlas_textures(std::ostream& stream, const GodotScene& scene)
{
  if (!scene.atlas_textures().empty()) {
    for (const auto& [id, texture] : scene.atlas_textures()) {
      stream << '\n';
      stream << fmt::format("[sub_resource type=\"AtlasTexture\" id={}]\n", id);
      stream << fmt::format("atlas = ExtResource( {} )\n", texture.atlas_id);
      stream << fmt::format("region = Rect2( {}, {}, {}, {} )\n",
                            texture.region.x(),
                            texture.region.y(),
                            texture.region.z(),
                            texture.region.w());
    }
  }
}

void write_sprite_frames(std::ostream& stream, const GdSpriteFrames& sprite_frames)
{
  stream << fmt::format("\n[sub_resource type=\"SpriteFrames\" id={}]\n",
                        sprite_frames.id);
  stream << "animations = [\n";

  for (const auto& animation : sprite_frames.animations) {
    stream << "  {\n";

    stream << "    \"frames\": [ ";
    bool first_frame = true;
    for (const auto& frame : animation.frames) {
      if (!first_frame) {
        stream << ", ";
      }
      stream << fmt::format("SubResource( {} )", frame);
      first_frame = false;
    }
    stream << " ],\n";

    stream << "    \"loop\": true,\n";
    stream << fmt::format("    \"name\": \"{}\",\n", animation.name);
    stream << fmt::format("    \"speed\": \"{:.3f}\"\n", animation.speed);

    stream << "  }\n";
  }
  stream << "]\n";
}

void write_shapes(std::ostream& stream, const GodotScene& scene)
{
  for (const auto& [id, shape] : scene.rectangle_shapes()) {
    stream << fmt::format("\n[sub_resource type=\"RectangleShape2D\" id={}]\n", id);
    stream << fmt::format("extents = Vector2( {}, {} )\n",
                          shape.extents.x(),
                          shape.extents.y());
  }
}

void write_tile_layer_animation_nodes(std::ostream& stream,
                                      const GodotScene& scene,
                                      const GdTileLayer& tile_layer)
{
  const auto& sprite_frames = scene.sprite_frames();

  for (const auto& animation : tile_layer.animations) {
    const auto name = fmt::format("Tile ({}, {})", animation.row, animation.col);
    stream << fmt::format("\n[node name=\"{}\" type=\"AnimatedSprite\" parent=\"{}\"]\n",
                          name,
                          animation.parent);

    const auto pos = TilePos::from(animation.row, animation.col);
    const auto x = pos.col_to_x(tile_layer.cell_size.x());
    const auto y = pos.row_to_y(tile_layer.cell_size.y());

    stream << fmt::format("position = Vector2( {}, {} )\n", x, y);
    stream << fmt::format("frames = SubResource( {} )\n", sprite_frames.id);
    stream << "speed_scale = 1.0\n";
    stream << fmt::format("animation = \"Tile {}\"\n", animation.tile_id);
    stream << "playing = true\n";
    stream << "centered = false\n";
  }
}

void write_tile_layer(std::ostream& stream, const GodotScene& scene, const GdLayer& layer)
{
  const auto& tile_layer = std::get<GdTileLayer>(layer.value);

  stream << fmt::format("\n[node name=\"{}\" type=\"TileMap\" parent=\"{}\"]\n",
                        layer.name,
                        layer.parent);
  stream << fmt::format("tile_set = ExtResource( {} )\n", scene.tileset_id());
  stream << "format = 1\n";
  if (!layer.visible) {
    stream << "visible = false\n";
  }

  stream << "tile_data = PoolIntArray( ";

  bool first_tile = true;
  for (const auto& encoded_tile : tile_layer.data) {
    if (!first_tile) {
      stream << ", ";
    }

    stream << fmt::format("{}, {}, {}",
                          encoded_tile.position,
                          encoded_tile.tileset,
                          encoded_tile.tile_index);

    first_tile = false;
  }

  stream << " )\n";
  stream << fmt::format("cell_size = Vector2( {}, {} )\n",
                        tile_layer.cell_size.x(),
                        tile_layer.cell_size.y());

  write_metadata(stream, layer.meta);

  write_tile_layer_animation_nodes(stream, scene, tile_layer);
}

void write_rectangle_object(std::ostream& stream, const GdObject& object)
{
  const auto& rect = std::get<GdRect>(object.value);

  stream << fmt::format("\n[node name=\"{}\" type=\"Area2D\" parent=\"{}\"]\n",
                        object.name,
                        object.parent);
  stream << fmt::format("position = Vector2( {:.3f}, {:.3f} )\n",
                        object.position.x(),
                        object.position.y());
  if (!object.visible) {
    stream << "visible = false\n";
  }

  write_metadata(stream, object.meta);

  stream << fmt::format(
      "\n[node name=\"Shape\" type=\"CollisionShape2D\" parent=\"{}/{}\"]\n",
      object.parent,
      object.name);
  stream << fmt::format("shape = SubResource( {} )\n", rect.shape);
}

void write_polygon_object(std::ostream& stream, const GdObject& object)
{
  const auto& polygon = std::get<GdPolygon>(object.value);

  stream << fmt::format("\n[node name=\"{}\" type=\"Area2D\" parent=\"{}\"]\n",
                        object.name,
                        object.parent);
  stream << fmt::format("position = Vector2( {:.3f}, {:.3f} )\n",
                        object.position.x(),
                        object.position.y());
  if (!object.visible) {
    stream << "visible = false\n";
  }

  write_metadata(stream, object.meta);

  stream << fmt::format(
      "\n[node name=\"Shape\" type=\"CollisionPolygon2D\" parent=\"{}/{}\"]\n",
      object.parent,
      object.name);

  stream << "polygon = PoolVector2Array( ";

  bool first_point = true;
  for (const auto& point : polygon.points) {
    if (!first_point) {
      stream << ", ";
    }
    stream << fmt::format("{:.3f}, {:.3f}", point.x(), point.y());
    first_point = false;
  }

  stream << " )\n";
}

void write_object(std::ostream& stream, const GdObject& object)
{
  if (std::holds_alternative<GdRect>(object.value)) {
    write_rectangle_object(stream, object);
  }
  else if (std::holds_alternative<GdPolygon>(object.value)) {
    write_polygon_object(stream, object);
  }
  else {
    stream << fmt::format("\n[node name=\"{}\" type=\"Node2D\" parent=\"{}\"]\n",
                          object.name,
                          object.parent);
    stream << fmt::format("position = Vector2( {:.3f}, {:.3f} )\n",
                          object.position.x(),
                          object.position.y());
    if (!object.visible) {
      stream << "visible = false\n";
    }

    write_metadata(stream, object.meta);
  }
}

void write_object_layer(std::ostream& stream, const GdLayer& layer)
{
  const auto& object_layer = std::get<GdObjectLayer>(layer.value);

  stream << fmt::format("\n[node name=\"{}\" type=\"Node2D\" parent=\"{}\"]\n",
                        layer.name,
                        layer.parent);
  if (!layer.visible) {
    stream << "visible = false\n";
  }

  write_metadata(stream, layer.meta);

  for (const auto& object : object_layer.objects) {
    write_object(stream, object);
  }
}

void write_layers(std::ostream& stream, const GodotScene& scene)
{
  for (const auto& layer : scene.layers()) {
    if (std::holds_alternative<GdTileLayer>(layer.value)) {
      write_tile_layer(stream, scene, layer);
    }
    else if (std::holds_alternative<GdObjectLayer>(layer.value)) {
      write_object_layer(stream, layer);
    }
    else {
      stream << fmt::format("\n[node name=\"{}\" type=\"Node2D\" parent=\"{}\"]\n",
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
  auto stream = open_output_stream(path, FileType::Text).value();

  stream << fmt::format("[gd_scene load_steps={} format=2]\n", scene.get_load_steps());

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

}  // namespace tactile
