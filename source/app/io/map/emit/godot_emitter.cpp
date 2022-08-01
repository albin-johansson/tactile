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

#include "io/map/emit/emit_info.hpp"
#include "io/map/emit/emitter.hpp"
#include "misc/logging.hpp"

// Reference: https://docs.godotengine.org/en/stable/development/file_formats/tscn.html

namespace tactile::io {
namespace {}

void emit_godot_map(const EmitInfo& info)
{
  spdlog::debug("Saving Godot scene to {}...",
                fs::absolute(info.destination_file()).string());

  const auto& data = info.data();
  const usize nResources = data.tilesets.size() + 1;

  std::ofstream stream {info.destination_file(), std::ios::out};
  stream << fmt::format("[gd_scene load_steps={} format=2]\n", nResources);

  for (int32 id = 1; const auto& tileset : data.tilesets) {
    stream << fmt::format(R"([ext_resource path="res://{}.tres" type="TileSet" id={}])",
                          tileset.name,
                          id);
  }

  stream << "\n[resource]\n";

  // This is the root node
  stream << fmt::format(R"([node name="{}" type="Node2D"])",
                        info.destination_file().filename().string());

  auto write_node = [](std::string_view name,
                       std::string_view type,
                       std::string_view parent) {
    return fmt::format(R"([node name="{}" type="{}" parent="{}"])", name, type, parent);
  };

  for (const auto& layer : data.layers) {
    switch (layer.type) {
      case LayerType::TileLayer:
        stream << write_node(layer.name, "TileMap", ".") << '\n';
        break;

      case LayerType::ObjectLayer:
        [[fallthrough]];
      case LayerType::GroupLayer:
        stream << write_node(layer.name, "Node2D", ".") << '\n';
        break;
    }
  }
}

}  // namespace tactile::io