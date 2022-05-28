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
#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "editor/ui/alignment.hpp"
#include "editor/ui/dialogs/about_dialog.hpp"
#include "editor/ui/dialogs/credits_dialog.hpp"
#include "editor/ui/icons.hpp"
#include "editor/ui/scoped.hpp"

namespace tactile {
namespace {

struct HelpMenuState final
{
  AboutDialog about_dialog;
  CreditsDialog credits_dialog;
  bool show_about_imgui{};
};

[[nodiscard]] auto _get_state() -> HelpMenuState&
{
  static HelpMenuState state;
  return state;
}

}  // namespace

void update_help_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  auto& state = _get_state();

  if (scoped::Menu menu{"Help"}; menu.is_open()) {
    if (ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...")) {
      state.about_dialog.show();
    }

    state.show_about_imgui = ImGui::MenuItem("About Dear ImGui...");

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_BUG " Report Issue...")) {
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Credits...")) {
      state.credits_dialog.show();
    }
  }

  state.about_dialog.update(model, dispatcher);
  state.credits_dialog.update(model, dispatcher);

  if (state.show_about_imgui) {
    center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&state.show_about_imgui);
  }
}

}  // namespace tactile
