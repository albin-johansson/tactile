// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "set_layer_visible.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

SetLayerVisible::SetLayerVisible(Shared<Map> map,
                                 const UUID& layer_id,
                                 const bool visible)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mNewVisibility {visible}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void SetLayerVisible::undo()
{
  auto& layer = mMap->get_invisible_root().get_layer(mLayerId);

  layer.set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void SetLayerVisible::redo()
{
  auto& layer = mMap->get_invisible_root().get_layer(mLayerId);

  mOldVisibility = layer.is_visible();
  layer.set_visible(mNewVisibility);
}

auto SetLayerVisible::get_name() const -> String
{
  const auto& lang = get_current_language();
  return mNewVisibility ? lang.cmd.show_layer : lang.cmd.hide_layer;
}

}  // namespace tactile::cmd
