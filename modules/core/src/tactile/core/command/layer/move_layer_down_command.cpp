// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/move_layer_down_command.hpp"

#include <fmt/ostream.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

MoveLayerDownCommand::MoveLayerDownCommand(MapDocument* document, const UUID& layer_uuid)
  : mDocument {require_not_null(document, "null document")},
    mLayerUuid {layer_uuid}
{}

void MoveLayerDownCommand::undo()
{
  TACTILE_LOG_TRACE("[MoveLayerDownCommand::undo] layer: {}", fmt::streamed(mLayerUuid));

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  root_layer.move_layer_up(mLayerUuid);
}

void MoveLayerDownCommand::redo()
{
  TACTILE_LOG_TRACE("[MoveLayerDownCommand::redo] layer: {}", fmt::streamed(mLayerUuid));

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  root_layer.move_layer_down(mLayerUuid);
}

}  // namespace tactile::core
