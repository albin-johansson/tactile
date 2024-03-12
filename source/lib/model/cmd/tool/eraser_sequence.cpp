// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "eraser_sequence.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

EraserSequence::EraserSequence(Shared<Map> map, const UUID& layer_id, TileCache old_state)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mOldState {std::move(old_state)}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void EraserSequence::undo()
{
  auto& layer = mMap->get_invisible_root().get_tile_layer(mLayerId);

  for (const auto& [pos, tile]: mOldState) {
    layer.set_tile(pos, tile);
  }
}

void EraserSequence::redo()
{
  auto& layer = mMap->get_invisible_root().get_tile_layer(mLayerId);

  for (const auto& [position, _]: mOldState) {
    layer.set_tile(position, kEmptyTile);
  }
}

auto EraserSequence::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.eraser_tool;
}

}  // namespace tactile::cmd
