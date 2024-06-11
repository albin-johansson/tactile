// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {
class WidgetManager;
}  // namespace ui

struct ShowNewMapDialogEvent;
struct ShowOpenMapDialogEvent;
struct CreateMapEvent;

/**
 * Handles events related to maps.
 */
class MapEventHandler final
{
 public:
  /**
   * Creates a map event handler.
   *
   * \param model          The associated model, cannot be null.
   * \param widget_manager The associated widget manager, cannot be null.
   */
  MapEventHandler(Model* model, ui::WidgetManager* widget_manager);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Opens the map creation dialog.
   *
   * \param event The associated event.
   */
  void on_show_new_map_dialog(const ShowNewMapDialogEvent& event);

  /**
   * Opens the map selector dialog.
   *
   * \param event The associated event.
   */
  void on_show_open_map_dialog(const ShowOpenMapDialogEvent& event);

  /**
   * Creates a new map.
   *
   * \param event The associated event.
   */
  void on_create_map(const CreateMapEvent& event);

 private:
  Model* mModel;
  ui::WidgetManager* mWidgetManager;
};

}  // namespace tactile
