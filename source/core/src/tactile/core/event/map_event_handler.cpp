// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/map_event_handler.hpp"

#include <magic_enum.hpp>

#include "tactile/base/numeric/vec_format.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/dialog_events.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/map_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/widget_manager.hpp"

namespace tactile {

MapEventHandler::MapEventHandler(Model* model,
                                 ui::WidgetManager* widget_manager)
  : mModel {require_not_null(model, "null model")},
    mWidgetManager {require_not_null(widget_manager, "null widget manager")}
{}

void MapEventHandler::install(EventDispatcher& dispatcher)
{
  TACTILE_LOG_DEBUG("Installing map event handler");

  using Self = MapEventHandler;
  dispatcher.bind<ShowNewMapDialogEvent, &Self::on_show_new_map_dialog>(this);
  dispatcher.bind<ShowOpenMapDialogEvent, &Self::on_show_open_map_dialog>(this);
  dispatcher.bind<CreateMapEvent, &Self::on_create_map>(this);
}

void MapEventHandler::on_show_new_map_dialog(const ShowNewMapDialogEvent&)
{
  TACTILE_LOG_TRACE("ShowNewMapDialogEvent");
  mWidgetManager->get_new_map_dialog().open();
}

void MapEventHandler::on_show_open_map_dialog(const ShowOpenMapDialogEvent&)
{
  TACTILE_LOG_TRACE("ShowOpenMapDialogEvent");
}

void MapEventHandler::on_create_map(const CreateMapEvent& event)
{
  TACTILE_LOG_TRACE("CreateMapEvent(orientation: {}, size: {}, tile_size: {})",
                    magic_enum::enum_name(event.spec.orientation),
                    event.spec.extent,
                    event.spec.tile_size);
}

}  // namespace tactile
