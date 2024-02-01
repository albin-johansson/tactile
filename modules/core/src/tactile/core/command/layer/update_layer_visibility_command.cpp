// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/update_layer_visibility_command.hpp"

#include <fmt/ostream.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

UpdateLayerVisibilityCommand::UpdateLayerVisibilityCommand(MapDocument* document,
                                                           const UUID& layer_uuid,
                                                           const bool visibility)
  : mDocument {require_not_null(document, "null document")},
    mLayerUuid {layer_uuid},
    mNewVisibility {visibility}
{}

void UpdateLayerVisibilityCommand::undo()
{
  TACTILE_LOG_TRACE("[UpdateLayerVisibilityCommand::undo] layer: {}, new visibility: {}",
                    fmt::streamed(mLayerUuid),
                    mOldVisibility.value());

  auto& root_layer = mDocument->map().root_layer();
  auto* layer = require_not_null(root_layer.find_layer(mLayerUuid));

  layer->set_visible(mOldVisibility.value());
  mOldVisibility.reset();
}

void UpdateLayerVisibilityCommand::redo()
{
  TACTILE_LOG_TRACE("[UpdateLayerVisibilityCommand::redo] layer: {}, new visibility: {}",
                    fmt::streamed(mLayerUuid),
                    mNewVisibility);

  auto& root_layer = mDocument->map().root_layer();
  auto* layer = require_not_null(root_layer.find_layer(mLayerUuid));

  mOldVisibility = layer->is_visible();
  layer->set_visible(mNewVisibility);
}

}  // namespace tactile::core
