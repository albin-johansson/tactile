// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "help_menu.hpp"

#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void update_help_menu(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.help.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::AboutTactile);
    ui_menu_item(dispatcher, MenuAction::AboutDearImGui);

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::ReportIssue);

    ImGui::Separator();
    ui_menu_item(dispatcher, MenuAction::Credits);
  }
}

}  // namespace tactile::ui
