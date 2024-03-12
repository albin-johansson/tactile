// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "move_layer_up.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

MoveLayerUp::MoveLayerUp(Shared<Map> map, const UUID& layer_id)
    : mMap {std::move(map)},
      mLayerId {layer_id}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void MoveLayerUp::undo()
{
  mMap->get_invisible_root().move_layer_down(mLayerId);
}

void MoveLayerUp::redo()
{
  mMap->get_invisible_root().move_layer_up(mLayerId);
}

auto MoveLayerUp::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.move_layer_up;
}

}  // namespace tactile::cmd
