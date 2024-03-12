// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "fix_map_tiles.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

FixMapTiles::FixMapTiles(Shared<Map> map)
    : mMap {std::move(map)}
{
  if (!mMap) {
    throw Exception {"Invalid null map"};
  }
}

void FixMapTiles::undo()
{
  auto& root_layer = mMap->get_invisible_root();

  for (const auto& [layer_id, invalid_positions]: mInvalidLayerTiles) {
    auto& layer = root_layer.get_tile_layer(layer_id);

    for (const auto& [position, tile_id]: invalid_positions) {
      layer.set_tile(position, tile_id);
    }
  }

  mInvalidLayerTiles.clear();
}

void FixMapTiles::redo()
{
  mInvalidLayerTiles = mMap->fix_tiles();
}

auto FixMapTiles::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.fix_map_tiles;
}

}  // namespace tactile::cmd
