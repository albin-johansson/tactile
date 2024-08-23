// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"

namespace tactile {

class Model;
class IRenderer;
class EventDispatcher;

namespace ui {
class WidgetManager;
}  // namespace ui

struct ShowNewTilesetDialogEvent;
struct AddTilesetEvent;

/**
 * Handles events related to tilesets.
 */
class TilesetEventHandler final
{
 public:
  /**
   * Creates a tileset event handler.
   *
   * \param model          The associated model, cannot be null.
   * \param renderer       The associated renderer, cannot be null.
   * \param widget_manager The associated widget manager, cannot be null.
   */
  TilesetEventHandler(Model* model, IRenderer* renderer, ui::WidgetManager* widget_manager);

  /**
   * Installs the event handler to a given event dispatcher.
   *
   * \param dispatcher The target event dispatcher.
   */
  void install(EventDispatcher& dispatcher);

  /**
   * Opens the tileset creation dialog.
   *
   * \param event The associated event.
   */
  void on_show_new_tileset_dialog(const ShowNewTilesetDialogEvent& event);

  /**
   * Creates a new tileset.
   *
   * \param event The associated event.
   */
  void on_add_tileset(const AddTilesetEvent& event);

 private:
  Model* mModel;
  IRenderer* mRenderer;
  ui::WidgetManager* mWidgetManager;
};

}  // namespace tactile
