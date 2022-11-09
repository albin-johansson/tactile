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

#include <centurion/system.hpp>
#include <imgui.h>

#include "editor/ui/dialog/dialog_state.hpp"
#include "editor/ui/dialog/dialogs.hpp"
#include "editor/ui/style/alignment.hpp"
#include "editor/ui/style/icons.hpp"
#include "editor/ui/widget/scoped.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {
namespace {

constinit bool menu_show_about_imgui = false;

}  // namespace

void update_help_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (Menu menu {lang.menu.help.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(lang.action.show_about.c_str())) {
      get_dialogs().about.show();
    }

    menu_show_about_imgui = ImGui::MenuItem(lang.action.about_dear_imgui.c_str());

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.report_issue.c_str())) {
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    if (ImGui::MenuItem(lang.action.show_credits.c_str())) {
      get_dialogs().credits.show();
    }
  }

  auto& dialogs = get_dialogs();
  dialogs.about.update(model, dispatcher);
  dialogs.credits.update(model, dispatcher);

  if (menu_show_about_imgui) {
    center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&menu_show_about_imgui);
  }
}

}  // namespace tactile::ui
