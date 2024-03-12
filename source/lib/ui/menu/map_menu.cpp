// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "map_menu.hpp"

#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/model.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void update_map_menu(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const Disable disable {!model.is_map_active()};
  if (const Menu menu {lang.menu.map.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::InspectMap);

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::AddTileset, TACTILE_PRIMARY_MOD "+T");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::AddRow, TACTILE_SECONDARY_MOD "+R");
    ui_menu_item(dispatcher, MenuAction::AddColumn, TACTILE_SECONDARY_MOD "+C");
    ui_menu_item(dispatcher, MenuAction::RemoveRow, TACTILE_SECONDARY_MOD "+Shift+R");
    ui_menu_item(dispatcher, MenuAction::RemoveColumn, TACTILE_SECONDARY_MOD "+Shift+C");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::FixInvalidTiles);

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::ResizeMap);

    ImGui::Separator();

    if (const Menu export_menu {lang.menu.export_as.c_str()}; export_menu.is_open()) {
      ui_menu_item(dispatcher, MenuAction::ExportGodotScene);
    }
  }
}

}  // namespace tactile::ui
