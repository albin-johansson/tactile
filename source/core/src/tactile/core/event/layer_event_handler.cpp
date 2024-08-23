// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/layer_event_handler.hpp"

#include <magic_enum.hpp>

#include "tactile/core/debug/validation.hpp"
#include "tactile/core/document/map_document.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/layer_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"

namespace tactile {

LayerEventHandler::LayerEventHandler(Model* model) :
  mModel {require_not_null(model, "null model")}
{}

void LayerEventHandler::install(EventDispatcher& dispatcher)
{
  using Self = LayerEventHandler;
  dispatcher.bind<CreateLayerEvent, &Self::on_create_layer>(this);
  dispatcher.bind<RemoveLayerEvent, &Self::on_remove_layer>(this);
  dispatcher.bind<DuplicateLayerEvent, &Self::on_duplicate_layer>(this);
  dispatcher.bind<MoveLayerUpEvent, &Self::on_move_layer_up>(this);
  dispatcher.bind<MoveLayerDownEvent, &Self::on_move_layer_down>(this);
  dispatcher.bind<RenameLayerEvent, &Self::on_rename_layer>(this);
  dispatcher.bind<SetLayerOpacityEvent, &Self::on_set_layer_opacity>(this);
  dispatcher.bind<SetLayerVisibleEvent, &Self::on_set_layer_visible>(this);
}

void LayerEventHandler::on_create_layer(const CreateLayerEvent& event)
{
  TACTILE_LOG_TRACE("CreateLayerEvent(type: {})", magic_enum::enum_name(event.type));
}

void LayerEventHandler::on_remove_layer(const RemoveLayerEvent& event)
{
  TACTILE_LOG_TRACE("RemoveLayerEvent");
}

void LayerEventHandler::on_duplicate_layer(const DuplicateLayerEvent& event)
{
  TACTILE_LOG_TRACE("DuplicateLayerEvent");
}

void LayerEventHandler::on_move_layer_up(const MoveLayerUpEvent& event)
{
  TACTILE_LOG_TRACE("MoveLayerUpEvent");
}

void LayerEventHandler::on_move_layer_down(const MoveLayerDownEvent& event)
{
  TACTILE_LOG_TRACE("MoveLayerDownEvent");
}

void LayerEventHandler::on_rename_layer(const RenameLayerEvent& event)
{
  TACTILE_LOG_TRACE("RenameLayerEvent");
}

void LayerEventHandler::on_set_layer_opacity(const SetLayerOpacityEvent& event)
{
  TACTILE_LOG_TRACE("SetLayerOpacityEvent");
}

void LayerEventHandler::on_set_layer_visible(const SetLayerVisibleEvent& event)
{
  TACTILE_LOG_TRACE("SetLayerVisibleEvent");
}

}  // namespace tactile
