/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
