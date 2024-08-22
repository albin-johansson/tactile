// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/layer/create_layer_command.hpp"

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer_common.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"

namespace tactile {

CreateLayerCommand::CreateLayerCommand(MapDocument* document, const LayerType type)
  : mDocument {require_not_null(document, "null document")},
    mType {type},
    mParentLayerId {kInvalidEntity},
    mLayerId {kInvalidEntity},
    mWasLayerAdded {false}
{}

void CreateLayerCommand::undo()
{
  TACTILE_LOG_DEBUG("Removing layer {}", entity_to_string(mLayerId));

  auto& registry = mDocument->get_registry();

  const auto& document_info = registry.get<CDocumentInfo>();
  const auto map_id = document_info.root;

  remove_layer_from_map(registry, map_id, mLayerId).value();

  mWasLayerAdded = false;
}

void CreateLayerCommand::redo()
{
  auto& registry = mDocument->get_registry();

  const auto& document_info = registry.get<CDocumentInfo>();
  const auto map_id = document_info.root;

  auto& map = registry.get<CMap>(map_id);

  if (mLayerId == kInvalidEntity) {
    mParentLayerId = map.active_layer;
    mLayerId = add_layer_to_map(registry, map_id, mType).value();
  }
  else {
    const auto old_active_layer = map.active_layer;
    map.active_layer = mParentLayerId;

    append_layer_to_map(registry, map_id, mLayerId);

    map.active_layer = old_active_layer;
  }

  TACTILE_LOG_DEBUG("Created layer {}", entity_to_string(mLayerId));
  mWasLayerAdded = true;
}

void CreateLayerCommand::dispose()
{
  if (!mWasLayerAdded && mLayerId != kInvalidEntity) {
    auto& registry = mDocument->get_registry();
    destroy_layer(registry, mLayerId);

    mLayerId = kInvalidEntity;
  }
}

}  // namespace tactile
