// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/tileset_event_handler.hpp"

#include "tactile/base/render/renderer.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/tileset_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/widget_manager.hpp"

namespace tactile {

TilesetEventHandler::TilesetEventHandler(Model* model,
                                         IRenderer* renderer,
                                         ui::WidgetManager* widget_manager)
  : mModel {require_not_null(model, "null model")},
    mRenderer {require_not_null(renderer, "null renderer")},
    mWidgetManager {require_not_null(widget_manager, "null widget manager")}
{}

void TilesetEventHandler::install(EventDispatcher& dispatcher)
{
  using Self = TilesetEventHandler;

  // clang-format off
  dispatcher.bind<ShowNewTilesetDialogEvent, &Self::on_show_new_tileset_dialog>(this);
  dispatcher.bind<AddTilesetEvent, &Self::on_add_tileset>(this);
  // clang-format on
}

void TilesetEventHandler::on_show_new_tileset_dialog(
    const ShowNewTilesetDialogEvent&)
{
  TACTILE_LOG_TRACE("ShowNewTilesetDialogEvent");
  mWidgetManager->get_new_tileset_dialog().open();
}

void TilesetEventHandler::on_add_tileset(const AddTilesetEvent& event)
{
  TACTILE_LOG_TRACE("AddTilesetEvent");
}

}  // namespace tactile
