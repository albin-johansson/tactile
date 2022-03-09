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

#include "menu_bar.hpp"

#include <imgui.h>

#include "editor/model.hpp"
#include "map_menu.hpp"

namespace tactile {

void MenuBar::Update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    mFileMenu.Update(model, dispatcher);
    mEditMenu.Update(model, dispatcher);
    mViewMenu.Update(model, dispatcher);
    mMapMenu.Update(model, dispatcher);
    mHelpMenu.Update(model, dispatcher);
    mDebugMenu.Update();

    ImGui::EndMainMenuBar();
  }

  mFileMenu.UpdateWindows(model, dispatcher);
  mEditMenu.UpdateWindows(model, dispatcher);
  mHelpMenu.UpdateWindows();
  mDebugMenu.UpdateWindows();
}

void MenuBar::ShowSettings()
{
  mEditMenu.OpenSettingsModal();
}

void MenuBar::ShowNewMapDialog()
{
  mFileMenu.ShowNewMapDialog();
}

void MenuBar::ShowOpenMapDialog()
{
  mFileMenu.ShowOpenMapDialog();
}

void MenuBar::ShowAddTilesetDialog()
{
  mMapMenu.ShowAddTilesetDialog();
}

void MenuBar::ShowComponentEditor(const DocumentModel& model)
{
  mEditMenu.ShowComponentEditor(model);
}

}  // namespace tactile
