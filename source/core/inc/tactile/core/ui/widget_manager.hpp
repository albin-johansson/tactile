// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/dialog/new_map_dialog.hpp"
#include "tactile/core/ui/dialog/new_property_dialog.hpp"
#include "tactile/core/ui/dialog/new_tileset_dialog.hpp"
#include "tactile/core/ui/dialog/rename_property_dialog.hpp"
#include "tactile/core/ui/dock/component_dock.hpp"
#include "tactile/core/ui/dock/dock_space.hpp"
#include "tactile/core/ui/dock/document_dock.hpp"
#include "tactile/core/ui/dock/layer_dock.hpp"
#include "tactile/core/ui/dock/log_dock.hpp"
#include "tactile/core/ui/dock/property_dock.hpp"
#include "tactile/core/ui/dock/tileset_dock.hpp"
#include "tactile/core/ui/menu/menu_bar.hpp"

namespace tactile {

class Model;
class EventDispatcher;

namespace ui {

/**
 * Manages the UI state.
 */
class WidgetManager final
{
 public:
  /**
   * Pushes the active widgets to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The associated event dispatcher.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

  /**
   * Returns the dock space manager.
   *
   * \return
   * The associated dock space.
   */
  [[nodiscard]]
  auto get_dock_space() -> DockSpace&;

  /**
   * Returns the map creation dialog.
   *
   * \return
   * The associated map creation dialog.
   */
  [[nodiscard]]
  auto get_new_map_dialog() -> NewMapDialog&;

  /**
   * Returns the tileset creation dialog.
   *
   * \return
   * The associated tileset creation dialog.
   */
  [[nodiscard]]
  auto get_new_tileset_dialog() -> NewTilesetDialog&;

  /**
   * Returns the property creation dialog.
   *
   * \return
   * The associated property creation dialog.
   */
  [[nodiscard]]
  auto get_new_property_dialog() -> NewPropertyDialog&;

  /**
   * Returns the property rename dialog.
   *
   * \return
   * The associated property rename dialog.
   */
  [[nodiscard]]
  auto get_rename_property_dialog() -> RenamePropertyDialog&;

 private:
  MenuBar mMenuBar {};
  DockSpace mDockSpace {};
  DocumentDock mDocumentDock {};
  TilesetDock mTilesetDock {};
  LayerDock mLayerDock {};
  PropertyDock mPropertyDock {};
  ComponentDock mComponentDock {};
  LogDock mLogDock {};
  NewMapDialog mNewMapDialog {};
  NewTilesetDialog mNewTilesetDialog {};
  NewPropertyDialog mNewPropertyDialog {};
  RenamePropertyDialog mRenamePropertyDialog {};
};

}  // namespace ui
}  // namespace tactile
