// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "remove_layer.hpp"

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

RemoveLayer::RemoveLayer(MapDocument* document, const UUID& layer_id)
    : mDocument {document}
{
  if (!mDocument) {
    throw Exception {"Invalid null map document!"};
  }

  mLayer = mDocument->get_map().get_invisible_root().find_shared_layer(layer_id);
}

void RemoveLayer::undo()
{
  auto& map = mDocument->get_map();
  map.insert_layer(mLayer, mIndex.value());

  mDocument->get_contexts().add_context(mLayer);
  mIndex.reset();
}

void RemoveLayer::redo()
{
  auto& map = mDocument->get_map();
  const auto layer_id = mLayer->get_uuid();

  mIndex = map.get_invisible_root().local_layer_index(layer_id);
  map.remove_layer(layer_id);

  mDocument->get_contexts().remove_context(layer_id);
}

auto RemoveLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.remove_layer;
}

}  // namespace tactile::cmd
