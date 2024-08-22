// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/layer/duplicate_layer_command.hpp"

#include <exception>  // exception

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"

namespace tactile {

DuplicateLayerCommand::DuplicateLayerCommand(MapDocument* document, const EntityID layer_id)
  : mDocument {require_not_null(document, "null document")},
    mLayerId {layer_id},
    mDuplicateLayerId {kInvalidEntity},
    mLayerWasAdded {false}
{}

DuplicateLayerCommand::~DuplicateLayerCommand() noexcept
{
  try {
    if (!mLayerWasAdded && mDuplicateLayerId != kInvalidEntity) {
      auto& registry = mDocument->get_registry();
      destroy_layer(registry, mDuplicateLayerId);
    }
  }
  catch (const std::exception& error) {
    TACTILE_LOG_ERROR("DuplicateLayerCommand destructor threw exception: {}", error.what());
  }
}

void DuplicateLayerCommand::undo()
{
  TACTILE_LOG_TRACE("Removing layer {} (duplicated from {})",
                    entity_to_string(mDuplicateLayerId),
                    entity_to_string(mLayerId));

  auto& registry = mDocument->get_registry();
  const auto map_id = registry.get<CDocumentInfo>().root;

  remove_layer_from_map(registry, map_id, mDuplicateLayerId).value();
  mLayerWasAdded = false;
}

void DuplicateLayerCommand::redo()
{
  TACTILE_LOG_TRACE("Duplicating layer {}", entity_to_string(mLayerId));

  auto& registry = mDocument->get_registry();
  const auto map_id = registry.get<CDocumentInfo>().root;

  auto& map = registry.get<CMap>(map_id);
  map.active_layer = find_parent_layer(registry, map.root_layer, mLayerId);

  if (mDuplicateLayerId == kInvalidEntity) {
    auto& id_cache = registry.get<CMapIdCache>(map_id);
    mDuplicateLayerId = copy_layer(registry, mLayerId, id_cache.next_layer_id);
  }

  append_layer_to_map(registry, map_id, mDuplicateLayerId);
  mLayerWasAdded = true;
}

}  // namespace tactile
