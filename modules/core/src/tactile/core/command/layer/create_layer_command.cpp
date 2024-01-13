// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/command/layer/create_layer_command.hpp"

#include <utility>  // move

#include <fmt/ostream.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/layer/tile_layer.hpp"
#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/log/logger.hpp"

namespace tactile {

CreateLayerCommand::CreateLayerCommand(MapDocument* document, const LayerType type)
  : mDocument {require_not_null(document, "null document")},
    mLayerType {type}
{}

void CreateLayerCommand::undo()
{
  TACTILE_LOG_TRACE("[CreateLayerCommand::undo]");

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  root_layer.remove_layer(mNewLayerUuid.value());
  mNewLayerUuid.reset();
}

void CreateLayerCommand::redo()
{
  TACTILE_LOG_TRACE("[CreateLayerCommand::redo]");

  auto& map = mDocument->map();
  auto& root_layer = map.root_layer();

  auto new_layer = _create_layer();
  mNewLayerUuid = new_layer->meta().uuid();

  // The active layer property is not tracked by commands, so we need to cache
  // it to get reliable results.
  if (mNewLayerParentUuid.has_value()) {
    root_layer.append_layer_to(*mNewLayerParentUuid, std::move(new_layer));
  }
  else {
    const auto active_layer_uuid = map.active_layer_uuid();
    const auto has_active_group_layer =
        active_layer_uuid.has_value() &&
        dynamic_cast<const GroupLayer*>(root_layer.find_layer(*active_layer_uuid));

    if (has_active_group_layer) {
      mNewLayerParentUuid = active_layer_uuid;

      const auto succeeded =
          root_layer.append_layer_to(*active_layer_uuid, std::move(new_layer));
      if (!succeeded) {
        TACTILE_LOG_ERROR("[CreateLayerCommand] Could not add new layer to {}",
                          fmt::streamed(*active_layer_uuid));
      }
    }
    else {
      root_layer.append_layer(std::move(new_layer));
    }
  }
}

auto CreateLayerCommand::_create_layer() -> Shared<ILayer>
{
  switch (mLayerType) {
    case LayerType::kTileLayer: return make_shared<TileLayer>(mDocument->map().extent());
    case LayerType::kObjectLayer: return make_shared<ObjectLayer>();
    case LayerType::kGroupLayer: return make_shared<GroupLayer>();
    default: throw Exception {"invalid layer type"};
  }
}

}  // namespace tactile
