/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include <imgui.h>

#include "model/events/menu_events.hpp"
#include "model/services/language_service.hpp"
#include "model/services/service_locator.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile {

void push_help_menu(ModelView& model)
{
  const auto& strings = model.get_language_strings();

  if (const ui::Menu menu {strings.menu.help.c_str()}; menu.is_open()) {
    if (ImGui::MenuItem(strings.action.show_about.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ShowAbout))) {
      model.enqueue<MenuActionEvent>(MenuAction::ShowAbout);
    }

    if (ImGui::MenuItem(strings.action.about_dear_imgui.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ShowAboutImGui))) {
      model.enqueue<MenuActionEvent>(MenuAction::ShowAboutImGui);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.report_issue.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ReportIssue))) {
      model.enqueue<MenuActionEvent>(MenuAction::ReportIssue);
    }

    ImGui::Separator();

    if (ImGui::MenuItem(strings.action.show_credits.c_str(),
                        nullptr,
                        nullptr,
                        model.is_available(MenuAction::ShowCredits))) {
      model.enqueue<MenuActionEvent>(MenuAction::ShowCredits);
    }
  }
}

}  // namespace tactile
