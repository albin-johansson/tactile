// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/event/property_event_handler.hpp"

#include "tactile/core/cmd/meta/create_property_command.hpp"
#include "tactile/core/debug/validation.hpp"
#include "tactile/core/event/event_dispatcher.hpp"
#include "tactile/core/event/property_events.hpp"
#include "tactile/core/log/logger.hpp"
#include "tactile/core/model/model.hpp"
#include "tactile/core/ui/widget_manager.hpp"

namespace tactile {

PropertyEventHandler::PropertyEventHandler(Model* model,
                                           ui::WidgetManager* widget_manager)
  : mModel {require_not_null(model, "null model")},
    mWidgetManager {require_not_null(widget_manager, "null widget manager")}
{}

void PropertyEventHandler::install(EventDispatcher& dispatcher)
{
  using Self = PropertyEventHandler;

  // clang-format off
  dispatcher.bind<ShowNewPropertyDialogEvent, &Self::on_show_new_property_dialog>(this);
  dispatcher.bind<CreatePropertyEvent, &Self::on_create_property>(this);
  // clang-format on
}

void PropertyEventHandler::on_show_new_property_dialog(
    const ShowNewPropertyDialogEvent& event)
{
  TACTILE_LOG_TRACE("ShowNewPropertyDialogEvent(context: {})",
                    entity_to_string(event.context_entity));
  mWidgetManager->get_new_property_dialog().open(event.context_entity);
}

void PropertyEventHandler::on_create_property(const CreatePropertyEvent& event)
{
  TACTILE_LOG_TRACE("CreatePropertyEvent(context: {}, name: {})",
                    entity_to_string(event.context_entity),
                    event.name);
  mModel->push_command<CreatePropertyCommand>(event.context_entity,
                                              event.name,
                                              event.value);
}

}  // namespace tactile
