// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/cmd/layer/set_layer_opacity_command.hpp"

#include <utility>  // exchange

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/log/logger.hpp"

namespace tactile {

SetLayerOpacityCommand::SetLayerOpacityCommand(MapDocument* document,
                                               const EntityID layer_id,
                                               const float opacity)
  : mDocument {require_not_null(document, "null document")},
    mLayerId {layer_id},
    mNewOpacity {opacity},
    mOldOpacity {0.0f}
{}

void SetLayerOpacityCommand::undo()
{
  TACTILE_LOG_TRACE("Setting layer {} opacity to {}", entity_to_string(mLayerId), mOldOpacity);
  auto& registry = mDocument->get_registry();

  auto& layer = registry.get<CLayer>(mLayerId);
  layer.opacity = mOldOpacity;
}

void SetLayerOpacityCommand::redo()
{
  TACTILE_LOG_TRACE("Setting layer {} opacity to {}", entity_to_string(mLayerId), mNewOpacity);
  auto& registry = mDocument->get_registry();

  auto& layer = registry.get<CLayer>(mLayerId);
  mOldOpacity = std::exchange(layer.opacity, mNewOpacity);
}

auto SetLayerOpacityCommand::merge_with(const ICommand* cmd) -> bool
{
  const auto* other = dynamic_cast<const SetLayerOpacityCommand*>(cmd);

  if (!other || mDocument != other->mDocument || mLayerId != other->mLayerId) {
    return false;
  }

  mNewOpacity = other->mNewOpacity;

  return true;
}

}  // namespace tactile
