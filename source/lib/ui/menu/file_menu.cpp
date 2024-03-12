// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "file_menu.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/filesystem.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/map_events.hpp"
#include "model/file_history.hpp"
#include "ui/shortcut/mappings.hpp"
#include "ui/widget/scoped.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

void update_recent_files_menu(const Strings& lang, entt::dispatcher& dispatcher)
{
  if (const Menu menu {lang.menu.recent_files.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::ReopenLastClosedFile);

    const auto& history = get_file_history();
    if (!history.entries.empty()) {
      ImGui::Separator();
    }

    for (const auto& path: history.entries) {
      if (ImGui::MenuItem(path.c_str())) {
        // It's fine if the file doesn't exist anymore, the parser handles that.
        dispatcher.enqueue<OpenMapEvent>(Path {path});
      }
    }

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::ClearFileHistory);
  }
}

}  // namespace

void update_file_menu(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.file.c_str()}; menu.is_open()) {
    ui_menu_item(dispatcher, MenuAction::NewMap, TACTILE_PRIMARY_MOD "+N");
    ui_menu_item(dispatcher, MenuAction::OpenMap, TACTILE_PRIMARY_MOD "+O");

    update_recent_files_menu(lang, dispatcher);

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::Save, TACTILE_PRIMARY_MOD "+S");
    ui_menu_item(dispatcher, MenuAction::SaveAs, TACTILE_PRIMARY_MOD "+Shift+S");

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::CloseDocument);

    ImGui::Separator();

    ui_menu_item(dispatcher, MenuAction::Quit);
  }
}

}  // namespace tactile::ui
