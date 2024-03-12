// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "menu_bar.hpp"

#include <imgui.h>

#include "model/model.hpp"
#include "ui/menu/debug_menu.hpp"
#include "ui/menu/edit_menu.hpp"
#include "ui/menu/file_menu.hpp"
#include "ui/menu/help_menu.hpp"
#include "ui/menu/map_menu.hpp"
#include "ui/menu/tileset_menu.hpp"
#include "ui/menu/view_menu.hpp"

namespace tactile::ui {

void update_menu_bar(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  if (ImGui::BeginMainMenuBar()) {
    update_file_menu(dispatcher);
    update_edit_menu(dispatcher);
    update_view_menu(model, dispatcher);
    update_map_menu(model, dispatcher);
    update_tileset_menu(model, dispatcher);
    update_help_menu(dispatcher);
    update_debug_menu();

    ImGui::EndMainMenuBar();
  }
}

}  // namespace tactile::ui
