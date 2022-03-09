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

#include "help_menu.hpp"

#include <centurion.hpp>  // open_url
#include <imgui.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/dialogs/about_dialog.hpp"
#include "editor/gui/dialogs/credits_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

void HelpMenu::Update(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (scoped::menu menu{"Help"}; menu.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...")) {
      mAboutDialog.show();
    }

    mShowAboutImGui = ImGui::MenuItem(TAC_ICON_ABOUT " About Dear ImGui...");

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_BUG " Report Issue...")) {
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Credits...")) {
      mCreditsDialog.show();
    }
  }

  mAboutDialog.update(model, dispatcher);
  mCreditsDialog.update(model, dispatcher);
}

void HelpMenu::UpdateWindows()
{
  if (mShowAboutImGui) {
    center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&mShowAboutImGui);
  }
}

}  // namespace tactile
