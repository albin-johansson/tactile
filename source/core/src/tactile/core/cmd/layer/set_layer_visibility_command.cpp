// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/layer/set_layer_visibility_command.hpp"

#include <utility>  // exchange

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/log/logger.hpp"

namespace tactile {

SetLayerVisibilityCommand::SetLayerVisibilityCommand(MapDocument* document,
                                                     const EntityID layer_id,
                                                     const bool visibility)
  : mDocument {require_not_null(document, "null document")},
    mLayerId {layer_id},
    mNewVisibility {visibility},
    mOldVisibility {false}
{}

void SetLayerVisibilityCommand::undo()
{
  TACTILE_LOG_TRACE("Changing visibility of layer {} to {}",
                    entity_to_string(mLayerId),
                    mOldVisibility);

  auto& registry = mDocument->get_registry();
  auto& layer = registry.get<CLayer>(mLayerId);

  layer.visible = mOldVisibility;
}

void SetLayerVisibilityCommand::redo()
{
  TACTILE_LOG_TRACE("Changing visibility of layer {} to {}",
                    entity_to_string(mLayerId),
                    mNewVisibility);

  auto& registry = mDocument->get_registry();
  auto& layer = registry.get<CLayer>(mLayerId);

  mOldVisibility = std::exchange(layer.visible, mNewVisibility);
}

}  // namespace tactile
