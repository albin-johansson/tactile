#pragma once

#include <entt/entt.hpp>  // dispatcher

#include "debug_menu.hpp"
#include "edit_menu.hpp"
#include "file_menu.hpp"
#include "help_menu.hpp"
#include "map_menu.hpp"
#include "view_menu.hpp"

namespace tactile {

class document_model;

class MenuBar final
{
 public:
  /**
   * \brief Updates the state of the menu bar.
   *
   * \param model the associated model.
   * \param dispatcher the event dispatcher that will be used.
   */
  void Update(const document_model& model, entt::dispatcher& dispatcher);

  void ShowSettings();

  void ShowNewMapDialog();

  void ShowOpenMapDialog();

  void ShowAddTilesetDialog();

  void ShowComponentEditor(const document_model& model);

 private:
  FileMenu mFileMenu;
  EditMenu mEditMenu;
  ViewMenu mViewMenu;
  MapMenu mMapMenu;
  HelpMenu mHelpMenu;
  DebugMenu mDebugMenu;
};

}  // namespace tactile
