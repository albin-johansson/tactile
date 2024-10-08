// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {
class WidgetManager;
}  // namespace ui

struct ShowComponentEditorDialogEvent;
struct CreateComponentEvent;

/**
 * Handles events related to components.
 */
class ComponentEventHandler final
{
 public:
  /**
   * Creates a component event handler.
   *
   * \param model          The associated model, cannot be null.
   * \param widget_manager The associated widget manager, cannot be null.
   */
  ComponentEventHandler(Model* model, ui::WidgetManager* widget_manager);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Opens the component editor dialog.
   *
   * \param event The associated event.
   */
  void on_show_component_editor_dialog(const ShowComponentEditorDialogEvent& event);

  /**
   * Creates a new component.
   *
   * \param event The associated event.
   */
  void on_create_component(const CreateComponentEvent& event);

 private:
  Model* mModel;
  ui::WidgetManager* mWidgetManager;
};

}  // namespace tactile
