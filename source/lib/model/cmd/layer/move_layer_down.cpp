// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "move_layer_down.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

MoveLayerDown::MoveLayerDown(Shared<Map> map, const UUID& layer_id)
    : mMap {std::move(map)},
      mLayerId {layer_id}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void MoveLayerDown::undo()
{
  mMap->get_invisible_root().move_layer_up(mLayerId);
}

void MoveLayerDown::redo()
{
  mMap->get_invisible_root().move_layer_down(mLayerId);
}

auto MoveLayerDown::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.move_layer_down;
}

}  // namespace tactile::cmd
