// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/update_layer_opacity_command.hpp"

#include <fmt/ostream.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

UpdateLayerOpacityCommand::UpdateLayerOpacityCommand(MapDocument* document,
                                                     const UUID& layer_uuid,
                                                     const float opacity)
  : mDocument {require_not_null(document, "null document")},
    mLayerUuid {layer_uuid},
    mNewOpacity {opacity}
{}

void UpdateLayerOpacityCommand::undo()
{
  TACTILE_LOG_TRACE("[UpdateLayerOpacityCommand::undo] layer: {}, new opacity: {}",
                    fmt::streamed(mLayerUuid),
                    mOldOpacity.value());

  auto& map = mDocument->map();
  auto* layer = require_not_null(map.root_layer().find_layer(mLayerUuid));

  layer->set_opacity(mOldOpacity.value());
  mOldOpacity.reset();
}

void UpdateLayerOpacityCommand::redo()
{
  TACTILE_LOG_TRACE("[UpdateLayerOpacityCommand::redo] layer: {}, new opacity: {}",
                    fmt::streamed(mLayerUuid),
                    mNewOpacity);

  auto& map = mDocument->map();
  auto* layer = require_not_null(map.root_layer().find_layer(mLayerUuid));

  mOldOpacity = layer->get_opacity();
  layer->set_opacity(mNewOpacity);
}

auto UpdateLayerOpacityCommand::merge_with(const ICommand* other) -> bool
{
  if (const auto* that = dynamic_cast<const UpdateLayerOpacityCommand*>(other)) {
    if (this->mDocument == that->mDocument && this->mLayerUuid == that->mLayerUuid) {
      this->mNewOpacity = that->mNewOpacity;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
