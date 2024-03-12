// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "add_layer.hpp"

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

AddLayer::AddLayer(MapDocument* document, const LayerType type)
    : mDocument {document},
      mLayerType {type}
{
  if (!mDocument) {
    throw Exception {"Invalid null map document!"};
  }
}

void AddLayer::undo()
{
  auto& map = mDocument->get_map();
  const auto layer_id = mLayer->get_uuid();

  map.remove_layer(layer_id);
  mDocument->get_contexts().remove_context(layer_id);
}

void AddLayer::redo()
{
  auto& map = mDocument->get_map();

  if (!mLayer) {
    const auto active_layer_id = map.is_active_layer(LayerType::GroupLayer)
                                     ? map.get_active_layer_id()  //
                                     : nothing;
    const auto layer_id = map.add_layer(mLayerType, active_layer_id);
    mLayer = map.get_invisible_root().find_shared_layer(layer_id);
  }
  else {
    map.add_layer(mLayer, mLayer->get_parent());
  }

  mDocument->get_contexts().add_context(mLayer);
}

auto AddLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.add_layer;
}

}  // namespace tactile::cmd
