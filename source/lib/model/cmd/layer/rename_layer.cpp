// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_layer.hpp"

#include <utility>  // move

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RenameLayer::RenameLayer(Shared<Map> map, const UUID& layer_id, String name)
    : mMap {std::move(map)},
      mLayerId {layer_id},
      mNewName {std::move(name)}
{
  if (!mMap) {
    throw Exception {"Invalid null map!"};
  }
}

void RenameLayer::undo()
{
  auto& layer = mMap->get_invisible_root().get_layer(mLayerId);

  layer.get_ctx().set_name(mOldName.value());
  mOldName.reset();
}

void RenameLayer::redo()
{
  auto& layer = mMap->get_invisible_root().get_layer(mLayerId);
  auto& layer_ctx = layer.get_ctx();

  mOldName = layer_ctx.name();
  layer_ctx.set_name(mNewName);
}

auto RenameLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.rename_layer;
}

}  // namespace tactile::cmd
