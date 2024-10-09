// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#pragma once

#include "tactile/base/prelude.hpp"
#include "tactile/core/ui/menu/debug_menu.hpp"
#include "tactile/core/ui/menu/edit_menu.hpp"
#include "tactile/core/ui/menu/file_menu.hpp"
#include "tactile/core/ui/menu/help_menu.hpp"
#include "tactile/core/ui/menu/map_menu.hpp"
#include "tactile/core/ui/menu/tileset_menu.hpp"
#include "tactile/core/ui/menu/view_menu.hpp"

namespace tactile::core {

class Model;
class EventDispatcher;

namespace ui {

/**
 * Represents the main menu bar widget.
 */
class MenuBar final
{
 public:
  /**
   * Pushes the menu bar to the widget stack.
   *
   * \param model      The associated model.
   * \param dispatcher The event dispatcher to use.
   */
  void push(const Model& model, EventDispatcher& dispatcher);

 private:
  FileMenu mFileMenu {};
  EditMenu mEditMenu {};
  ViewMenu mViewMenu {};
  MapMenu mMapMenu {};
  TilesetMenu mTilesetMenu {};
  HelpMenu mHelpMenu {};
  DebugMenu mDebugMenu {};
};

}  // namespace ui
}  // namespace tactile::core
