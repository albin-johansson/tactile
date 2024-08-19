// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/layer/move_layer_up_command.hpp"

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/document_info.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/map/map.hpp"

namespace tactile {

MoveLayerUpCommand::MoveLayerUpCommand(MapDocument* document, const EntityID layer_id)
  : mDocument {require_not_null(document, "null document")},
    mLayerId {layer_id}
{}

void MoveLayerUpCommand::undo()
{
  TACTILE_LOG_TRACE("Moving layer {} down", entity_to_string(mLayerId));
  auto& registry = mDocument->get_registry();

  const auto map_id = registry.get<CDocumentInfo>().root;
  const auto& map = registry.get<CMap>(map_id);

  move_layer_down(registry, map.root_layer, mLayerId);
}

void MoveLayerUpCommand::redo()
{
  TACTILE_LOG_TRACE("Moving layer {} up", entity_to_string(mLayerId));
  auto& registry = mDocument->get_registry();

  const auto map_id = registry.get<CDocumentInfo>().root;
  const auto& map = registry.get<CMap>(map_id);

  move_layer_up(registry, map.root_layer, mLayerId);
}

}  // namespace tactile
