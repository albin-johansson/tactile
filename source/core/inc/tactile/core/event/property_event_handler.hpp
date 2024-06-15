// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {
class WidgetManager;
}  // namespace ui

struct ShowNewPropertyDialogEvent;
struct CreatePropertyEvent;

/**
 * Handles events related to meta context properties.
 */
class PropertyEventHandler final
{
 public:
  /**
   * Creates a property event handler.
   *
   * \param model          The associated model, cannot be null.
   * \param widget_manager The associated widget manager, cannot be null.
   */
  PropertyEventHandler(Model* model, ui::WidgetManager* widget_manager);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Opens the property creation dialog.
   *
   * \param event The associated event.
   */
  void on_show_new_property_dialog(const ShowNewPropertyDialogEvent& event);

  /**
   * Creates a new property.
   *
   * \param event The associated event.
   */
  void on_create_property(const CreatePropertyEvent& event);

 private:
  Model* mModel;
  ui::WidgetManager* mWidgetManager;
};

}  // namespace tactile
