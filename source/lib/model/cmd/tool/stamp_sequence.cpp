// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "stamp_sequence.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "core/layer/tile_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

StampSequence::StampSequence(Shared<Map> map,
                             const UUID& layer_id,
                             TileCache old_state,
                             TileCache new_state)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mOldState {std::move(old_state)},
      mNewState {std::move(new_state)}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void StampSequence::undo()
{
  apply_sequence(mOldState);
}

void StampSequence::redo()
{
  apply_sequence(mNewState);
}

auto StampSequence::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.stamp_tool;
}

void StampSequence::apply_sequence(const TileCache& cache)
{
  auto& layer = mMap->get_invisible_root().get_tile_layer(mLayerId);
  for (const auto& [pos, tile]: cache) {
    layer.set_tile(pos, tile);
  }
}

}  // namespace tactile::cmd
