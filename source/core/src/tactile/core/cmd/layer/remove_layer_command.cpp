// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/layer/remove_layer_command.hpp"

#include "tactile/core/debug/exception.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer_common.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"

namespace tactile {

RemoveLayerCommand::RemoveLayerCommand(MapDocument* document, const EntityID layer_id)
  : mDocument {require_not_null(document, "null document")},
    mLayerId {layer_id},
    mParentLayerId {kInvalidEntity},
    mLayerWasRemoved {false}
{}

void RemoveLayerCommand::undo()
{
  TACTILE_LOG_TRACE("Restoring layer {}", entity_to_string(mLayerId));
  auto& registry = mDocument->get_registry();

  const auto map_id = registry.get<CDocumentInfo>().root;
  auto& map = registry.get<CMap>(map_id);

  map.active_layer = mParentLayerId;
  append_layer_to_map(registry, map_id, mLayerId);

  mLayerWasRemoved = false;
}

void RemoveLayerCommand::redo()
{
  TACTILE_LOG_TRACE("Removing layer {}", entity_to_string(mLayerId));
  auto& registry = mDocument->get_registry();

  const auto map_id = registry.get<CDocumentInfo>().root;
  const auto& map = registry.get<CMap>(map_id);

  mParentLayerId = find_parent_layer(registry, map.root_layer, mLayerId);
  if (mParentLayerId == kInvalidEntity) {
    throw Exception {"No parent layer found for target layer"};
  }

  remove_layer_from_map(registry, map_id, mLayerId).value();
  mLayerWasRemoved = true;
}

void RemoveLayerCommand::dispose()
{
  if (mLayerWasRemoved && mLayerId != kInvalidEntity) {
    auto& registry = mDocument->get_registry();
    destroy_layer(registry, mLayerId);

    mLayerId = kInvalidEntity;
  }
}

}  // namespace tactile
