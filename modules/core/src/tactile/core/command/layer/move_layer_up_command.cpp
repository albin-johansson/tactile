// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/move_layer_up_command.hpp"

#include <fmt/ostream.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

MoveLayerUpCommand::MoveLayerUpCommand(MapDocument* document, const UUID& layer_uuid)
  : mDocument {require_not_null(document, "null document")},
    mLayerUuid {layer_uuid}
{}

void MoveLayerUpCommand::undo()
{
  TACTILE_LOG_TRACE("[MoveLayerUpCommand::undo] layer: {}", fmt::streamed(mLayerUuid));

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  root_layer.move_layer_down(mLayerUuid);
}

void MoveLayerUpCommand::redo()
{
  TACTILE_LOG_TRACE("[MoveLayerUpCommand::redo] layer: {}", fmt::streamed(mLayerUuid));

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  root_layer.move_layer_up(mLayerUuid);
}

}  // namespace tactile::core
