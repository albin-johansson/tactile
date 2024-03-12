// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "duplicate_layer.hpp"

#include "core/layer/group_layer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document/map_document.hpp"
#include "tactile/core/debug/exception.hpp"

namespace tactile::cmd {

DuplicateLayer::DuplicateLayer(MapDocument* document, const UUID& layer_id)
    : mDocument {document},
      mLayerId {layer_id}
{
  if (!mDocument) {
    throw Exception {"Invalid null map document"};
  }
}

void DuplicateLayer::undo()
{
  auto& map = mDocument->get_map();
  const auto layer_id = mNewLayer->get_uuid();

  map.remove_layer(layer_id);
  mDocument->get_contexts().remove_context(layer_id);
}

void DuplicateLayer::redo()
{
  auto& map = mDocument->get_map();

  if (mNewLayer != nullptr) {
    map.insert_layer(mNewLayer, mNewIndex.value());
  }
  else {
    mNewLayer = map.duplicate_layer(mLayerId);
    mNewIndex = map.get_invisible_root().local_layer_index(mNewLayer->get_uuid());
  }

  mDocument->get_contexts().add_context(mNewLayer);
}

auto DuplicateLayer::get_name() const -> String
{
  const auto& lang = get_current_language();
  return lang.cmd.duplicate_layer;
}

}  // namespace tactile::cmd
