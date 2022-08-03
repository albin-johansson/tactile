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

#include <fstream>      // ofstream
#include <ios>          // ios
#include <string_view>  // string_view

#include <fmt/format.h>

#include "core/common/functional.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"

// Reference: https://docs.godotengine.org/en/stable/development/file_formats/tscn.html

namespace tactile::io {
namespace {

/// Tracks different used identifiers
struct Identifiers final
{
  int32 next_ext_resource {1};
};

[[nodiscard]] auto write_node(std::string_view        name,
                              std::string_view        type,
                              Maybe<std::string_view> parent = nothing) -> std::string
{
  if (parent) {
    return fmt::format(R"([node name="{}" type="{}" parent="{}"])", name, type, *parent);
  }
  else {
    return fmt::format(R"([node name="{}" type="{}"])", name, type);
  }
}

[[nodiscard]] auto write_ext_resource(std::string_view path,
                                      std::string_view type,
                                      const auto&      id) -> std::string
{
  return fmt::format(R"([ext_resource path="res://{}" type="{}" id={}])", path, type, id);
}

void emit_external_tileset(const ir::TilesetData&  tileset,
                           const fs::path&         path,
                           const GodotEmitOptions& options)
{
  Identifiers   identifiers;
  std::ofstream stream {path, std::ios::out | std::ios::trunc};

  stream << fmt::format(R"([gd_resource type="TileSet" load_steps={} format=2])", 1)
         << '\n'
         << '\n';

  stream << fmt::format(
                R"([ext_resource path="res://{}" type="Texture" id={}])",
                fs::relative(tileset.image_path, options.project_image_dir).string(),
                identifiers.next_ext_resource++)
         << '\n';
}

void emit_tileset(std::ostream&           stream,
                  const ir::TilesetData&  tileset,
                  const GodotEmitOptions& options,
                  const int32             id)
{
  const auto filename = tileset.name + ".tres";

  if (options.embed_tilesets) {
    stream << write_ext_resource(filename, "TileSet", id) << '\n';
  }
  else {
    stream << write_ext_resource(filename, "TileSet", id) << '\n';

    const auto tileset_path =
        fs::absolute(options.root_dir) / options.project_map_dir / filename;
    emit_external_tileset(tileset, tileset_path, options);
  }
}

void emit_tilesets(std::ostream&           stream,
                   const ir::MapData&      map,
                   Identifiers&            identifiers,
                   const GodotEmitOptions& options)
{
  for (const auto& tileset : map.tilesets) {
    emit_tileset(stream, tileset, options, identifiers.next_ext_resource++);
  }
}

void emit_tile_layer(std::ostream&        stream,
                     const ir::MapData&   map,
                     const ir::LayerData& layer)
{
  const auto& tile_layer = std::get<ir::TileLayerData>(layer.data);

  stream << write_node(layer.name, "TileMap", ".") << '\n';
  stream << fmt::format("tile_set = ExtResource( {} )\n", 1);  // FIXME identifier
  stream << "format = 1\n";

  stream << "tile_data = PoolIntArray(";

  const auto rows = tile_layer.row_count;
  const auto columns = tile_layer.col_count;
  const auto tile_count = rows * columns;

  invoke_mn(rows, columns, [&, tile_count](usize row, usize col) {
    const auto index = row * tile_layer.col_count + col;
    stream << fmt::format((index < tile_count - 1) ? "{}, " : "{}",
                          tile_layer.tiles[row][col]);
  });

  stream << ")\n";

  stream << fmt::format("cell_size = Vector2( {}, {} )\n",
                        map.tile_size.x,
                        map.tile_size.y);

  stream << '\n';
}

void emit_layers(std::ostream& stream, const ir::MapData& map)
{
  stream << '\n';

  for (const auto& layer : map.layers) {
    switch (layer.type) {
      case LayerType::TileLayer:
        emit_tile_layer(stream, map, layer);
        break;

      case LayerType::ObjectLayer:
        [[fallthrough]];
      case LayerType::GroupLayer:
        stream << write_node(layer.name, "Node2D", ".") << '\n';
        break;
    }
  }
}

}  // namespace

void emit_godot_map(const EmitInfo& info, const GodotEmitOptions& options)
{
  const auto& map = info.data();
  const auto  resource_count = map.tilesets.size() + 1;  // FIXME

  std::ofstream stream {info.destination_file(), std::ios::out | std::ios::trunc};
  stream << fmt::format("[gd_scene load_steps={} format=2]\n\n", resource_count);

  // This is the root node
  stream << write_node(info.destination_file().stem().string(), "Node2D") << '\n' << '\n';

  Identifiers identifiers;
  emit_tilesets(stream, map, identifiers, options);
  emit_layers(stream, map);
}

}  // namespace tactile::io