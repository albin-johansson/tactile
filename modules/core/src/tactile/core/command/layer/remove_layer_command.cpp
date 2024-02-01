// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/remove_layer_command.hpp"

#include <fmt/ostream.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile::core {

RemoveLayerCommand::RemoveLayerCommand(MapDocument* document, const UUID& layer_uuid)
  : mDocument {require_not_null(document, "null document")},
    mLayerUuid {layer_uuid}
{}

void RemoveLayerCommand::undo()
{
  TACTILE_LOG_TRACE("[RemoveLayerCommand::undo] layer: {}", fmt::streamed(mLayerUuid));

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  const auto removed_layer_index = mRemovedLayerLocalIndex.value();

  if (mRemovedLayerParentUuid.has_value()) {
    root_layer.insert_layer_to(*mRemovedLayerParentUuid,
                               mRemovedLayer,
                               removed_layer_index);
  }
  else {
    root_layer.insert_layer(mRemovedLayer, removed_layer_index);
  }
}

void RemoveLayerCommand::redo()
{
  TACTILE_LOG_TRACE("[RemoveLayerCommand::redo] layer: {}", fmt::streamed(mLayerUuid));

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  if (const auto* parent_layer = root_layer.find_parent_layer(mLayerUuid)) {
    mRemovedLayerParentUuid = parent_layer->meta().uuid();
  }

  mRemovedLayerLocalIndex = root_layer.get_layer_local_index(mLayerUuid);
  mRemovedLayer = root_layer.remove_layer(mLayerUuid);
}

}  // namespace tactile::core
