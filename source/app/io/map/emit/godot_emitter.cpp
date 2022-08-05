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

#include <cmath>        // floor
#include <fstream>      // ofstream
#include <ios>          // ios
#include <string_view>  // string_view
#include <utility>      // pair
#include <vector>       // vector

#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "core/common/associative.hpp"
#include "core/common/functional.hpp"
#include "core/common/ints.hpp"
#include "core/common/maybe.hpp"
#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "misc/assert.hpp"

// Reference: https://docs.godotengine.org/en/stable/development/file_formats/tscn.html

namespace tactile::io {
namespace {

/// Tracks different used identifiers
struct Identifiers final
{
  int32        next_ext_resource {1};
  Maybe<int32> ext_tileset_id;
};

struct TilesetExportInfo final
{
  std::vector<usize>     sorted_indices;
  HashMap<TileID, int32> tileset_id_for_first_tile;
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

[[nodiscard]] auto get_tileset_export_info(const ir::MapData& map) -> TilesetExportInfo
{
  TilesetExportInfo info;

  std::vector<std::pair<usize, TileID>> pairs;
  pairs.reserve(map.tilesets.size());

  for (usize index = 0; const auto& tilesets : map.tilesets) {
    pairs.emplace_back(index, tilesets.first_tile);
    ++index;
  }

  std::sort(pairs.begin(), pairs.end(), [&](const auto& lhs, const auto& rhs) {
    return lhs.second < rhs.second;
  });

  info.sorted_indices.reserve(map.tilesets.size());
  for (const auto& [index, first_tile] : pairs) {
    info.sorted_indices.push_back(index);
    info.tileset_id_for_first_tile[first_tile] = static_cast<int32>(index) + 1;
  }

  return info;
}

void emit_tileset_file(const ir::MapData&       map,
                       const TilesetExportInfo& info,
                       const GodotEmitOptions&  options)
{
  // TODO copy images automatically? (maybe option)
  // TODO scrap embed tileset option?

  const auto load_steps = map.tilesets.size() + 1;  // TODO validate

  const auto tileset_file =
      options.root_dir / options.project_tileset_dir / "tileset.tres";

  std::ofstream stream {tileset_file, std::ios::out | std::ios::trunc};
  stream << fmt::format(R"([gd_resource type="TileSet" load_steps={} format=2])",
                        load_steps)
         << '\n'
         << '\n';

  for (const auto tileset_index : info.sorted_indices) {
    const auto& tileset = map.tilesets.at(tileset_index);

    const auto id = static_cast<int32>(tileset_index) + 1;
    const auto filename = tileset.name + tileset.image_path.extension().string();

    const auto image_path = options.project_image_dir / filename;
    fs::copy(tileset.image_path,
             options.root_dir / image_path,
             fs::copy_options::overwrite_existing);

    stream << fmt::format(R"([ext_resource path="res://{}" type="Texture" id={}])",
                          convert_to_forward_slashes(image_path),
                          id)
           << '\n';
  }

  stream << "\n[resource]\n";

  for (const auto tileset_index : info.sorted_indices) {
    const auto& tileset = map.tilesets.at(tileset_index);

    const auto id = static_cast<int32>(tileset_index) + 1;
    const auto prefix = fmt::format("{}/", id);

    stream << prefix << fmt::format("name = \"{}\"\n", tileset.name);
    stream << prefix << fmt::format("texture = ExtResource( {} )\n", id);
    stream << prefix << "tex_offset = Vector2( 0, 0 )\n";
    stream << prefix << "modulate = Color( 1, 1, 1, 1 )\n";
    stream << prefix
           << fmt::format("region = Rect2( 0, 0, {}, {} )\n",
                          tileset.image_size.x,
                          tileset.image_size.y);
    stream << prefix << "tile_mode = 2\n";
    stream << prefix << "autotile/icon_coordinate = Vector2( 0, 0 )\n";
    stream << prefix
           << fmt::format("autotile/tile_size = Vector2( {}, {} )\n",
                          tileset.tile_size.x,
                          tileset.tile_size.y);
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
  }
}

void emit_tilesets(std::ostream&            stream,
                   const ir::MapData&       map,
                   Identifiers&             identifiers,
                   const TilesetExportInfo& info,
                   const GodotEmitOptions&  options)
{
  const auto tileset_file = options.project_tileset_dir / "tileset.tres";
  identifiers.ext_tileset_id = identifiers.next_ext_resource++;

  stream << fmt::format(R"([ext_resource path="res://{}" type="TileSet" id={}])",
                        convert_to_forward_slashes(tileset_file),
                        identifiers.ext_tileset_id.value())
         << '\n';

  for (const auto& tileset : map.tilesets) {
    for (const auto& [tile_index, meta_tile] : tileset.fancy_tiles) {
      if (!meta_tile.frames.empty()) {
      }
    }
  }

  emit_tileset_file(map, info, options);
}

void emit_tile_layer(std::ostream&            stream,
                     const ir::MapData&       map,
                     const ir::LayerData&     layer,
                     Identifiers&             identifiers,
                     const TilesetExportInfo& info)
{
  const auto& tile_layer = std::get<ir::TileLayerData>(layer.data);

  stream << write_node(layer.name, "TileMap", ".") << '\n';
  stream << fmt::format("tile_set = ExtResource( {} )\n",
                        identifiers.ext_tileset_id.value());
  stream << "format = 1\n";

  stream << "tile_data = PoolIntArray(";

  bool has_emitted_tile = false;

  invoke_mn(tile_layer.row_count, tile_layer.col_count, [&](usize row, usize col) {
    const auto tile_id = tile_layer.tiles[row][col];

    if (tile_id != empty_tile) {
      const auto x = static_cast<int32>(col);
      auto       y = static_cast<int32>(row);

      if (x < 0) {
        y += 1;
      }

      constexpr int32 tile_offset = 65'536;

      const auto tileset =
          std::find_if(map.tilesets.begin(),
                       map.tilesets.end(),
                       [tile_id](const ir::TilesetData& tileset) {
                         return tile_id >= tileset.first_tile &&
                                tile_id <= tileset.first_tile + tileset.tile_count;
                       });
      TACTILE_ASSERT(tileset != map.tilesets.end());

      const auto parent_tileset =
          lookup_in(info.tileset_id_for_first_tile, tileset->first_tile);

      const int32 encoded_pos = x + (y * tile_offset);
      int32       encoded_tile = tile_id;

      const auto tile_index = tile_id - tileset->first_tile;

      if (tile_index >= tileset->column_count) {
        const auto tile_y =
            static_cast<int32>(std::floor(tile_index / tileset->column_count));
        const auto tile_x = tile_index % tileset->column_count;
        encoded_tile = tile_x + tile_y * tile_offset;
      }

      if (has_emitted_tile) {
        stream << ", ";
      }

      stream << fmt::format("{}, {}, {}", encoded_pos, parent_tileset, encoded_tile);
      has_emitted_tile = true;
    }
  });

  stream << ")\n";

  stream << fmt::format("cell_size = Vector2( {}, {} )\n",
                        map.tile_size.x,
                        map.tile_size.y);

  stream << '\n';
}

void emit_layers(std::ostream&            stream,
                 const ir::MapData&       map,
                 Identifiers&             identifiers,
                 const TilesetExportInfo& info)
{
  stream << '\n';

  for (const auto& layer : map.layers) {
    switch (layer.type) {
      case LayerType::TileLayer:
        emit_tile_layer(stream, map, layer, identifiers, info);
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

  const auto export_info = get_tileset_export_info(map);

  Identifiers identifiers;
  emit_tilesets(stream, map, identifiers, export_info, options);

  // This is the root node
  stream << '\n' << write_node(info.destination_file().stem().string(), "Node2D") << '\n';

  emit_layers(stream, map, identifiers, export_info);
}

}  // namespace tactile::io