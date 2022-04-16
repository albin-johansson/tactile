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

#include "godot_emitter.hpp"

#include <fstream>      // ofstream
#include <ios>          // ios
#include <string_view>  // string_view

#include <fmt/format.h>

#include "io/maps/emitter/emit_info.hpp"
#include "misc/logging.hpp"

namespace tactile::emitter {
namespace {}

void emit_godot_scene(const EmitInfo& info)
{
  log_debug("Saving Godot scene to {}...", absolute(info.destination_file()));

  const auto& data = info.data();
  const usize nResources = data.tilesets.size() + 1;

  std::ofstream stream{info.destination_file(), std::ios::out};
  stream << fmt::format("[gd_scene load_steps={} format=2]\n", nResources);

  for (int32 id = 1; const auto& tset : data.tilesets) {
    stream << fmt::format(
        "\n[ext_resource path=\"res://{}.tres\" type=\"TileSet\" id={}]\n",
        tset.name,
        id);
  }

  stream << "\n[resource]\n";

  /* This is the root node */
  stream << fmt::format("\n[node name=\"{}\" type=\"Node2D\"]\n",
                        info.destination_file().filename());

  auto writeNode = [](const std::string_view name,
                      const std::string_view type,
                      const std::string_view parent) {
    return fmt::format("\n[node name=\"{}\" type=\"{}\" parent=\"{}\"]",
                       name,
                       type,
                       parent);
  };

  for (const auto& layer : data.layers) {
    switch (layer.type) {
      case LayerType::TileLayer:
        stream << writeNode(layer.name, "TileMap", ".") << '\n';
        break;

      case LayerType::ObjectLayer:
        [[fallthrough]];
      case LayerType::GroupLayer:
        stream << writeNode(layer.name, "Node2D", ".") << '\n';
        break;
    }
  }
}

}  // namespace tactile::emitter