// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "ui.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <spdlog/spdlog.h>
#include <ui/style/alignment.hpp>

#include "model/event/map_events.hpp"
#include "model/model.hpp"
#include "tactile/base/container/path.hpp"
#include "tactile/core/platform/file_dialog.hpp"
#include "tactile/core/platform/filesystem.hpp"
#include "ui/dialog/about_dialog.hpp"
#include "ui/dialog/create_map_dialog.hpp"
#include "ui/dialog/credits_dialog.hpp"
#include "ui/dialog/godot_export_dialog.hpp"
#include "ui/dialog/map_parse_error_dialog.hpp"
#include "ui/dialog/resize_map_dialog.hpp"
#include "ui/dialog/settings_dialog.hpp"
#include "ui/dock/animation/animation_dock.hpp"
#include "ui/dock/comp/component_dock.hpp"
#include "ui/dock/comp/component_editor.hpp"
#include "ui/dock/dock_space.hpp"
#include "ui/dock/layer/layer_dock.hpp"
#include "ui/dock/log/log_dock.hpp"
#include "ui/dock/property/property_dock.hpp"
#include "ui/dock/tileset/dialogs/create_tileset_dialog.hpp"
#include "ui/dock/tileset/tileset_dock.hpp"
#include "ui/menu/menu_bar.hpp"
#include "ui/style/colors.hpp"
#include "ui/viewport/map_viewport_toolbar.hpp"
#include "ui/viewport/viewport_widget.hpp"

namespace tactile::ui {
namespace {

inline constinit bool gOpenMapFileDialog = false;
inline constinit bool gOpenAboutImGuiDialog = false;

void check_for_missing_ini_file()
{
  const auto& ini = get_imgui_ini_file_path();
  if (!fs::exists(ini)) {
    spdlog::warn("Resetting layout because 'imgui.ini' is missing...");
    reset_layout();

    const auto str = ini.string();
    ImGui::SaveIniSettingsToDisk(str.c_str());
  }
}

void update_map_file_dialog(entt::dispatcher& dispatcher)
{
  auto path = FileDialog::open_map();

  if (path.has_value()) {
    dispatcher.enqueue<OpenMapEvent>(std::move(*path));
  }

  gOpenMapFileDialog = false;
}

}  // namespace

void update_widgets(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  update_dynamic_color_cache();

  update_menu_bar(model, dispatcher);
  update_dock_space();

  const auto* document = model.active_document();

  if (document) {
    if (document->is_map()) {
      update_layer_dock(model, dispatcher);
      update_tileset_dock(model, dispatcher);
    }
    else if (document->is_tileset()) {
      ui_animation_dock(model, dispatcher);
    }

    update_property_dock(model, dispatcher);
    update_component_dock(model, dispatcher);
    update_log_dock();
  }

  update_viewport_widget(model, dispatcher);

  update_component_editor_dialog(model, dispatcher);
  update_create_map_dialog(dispatcher);
  update_create_tileset_dialog(dispatcher);
  update_resize_map_dialog(dispatcher);
  update_godot_export_dialog(dispatcher);
  update_settings_dialog(dispatcher);
  update_map_parse_error_dialog();
  update_credits_dialog();
  update_about_dialog();

  if (gOpenMapFileDialog) {
    update_map_file_dialog(dispatcher);
  }

  if (gOpenAboutImGuiDialog) {
    center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&gOpenAboutImGuiDialog);
  }

  check_for_missing_ini_file();
}

void open_map_file_dialog()
{
  gOpenMapFileDialog = true;
}

void open_about_dear_imgui_dialog()
{
  gOpenAboutImGuiDialog = true;
}

auto is_editor_focused() -> bool
{
  return is_map_toolbar_focused() || is_viewport_focused() || is_layer_dock_focused() ||
         is_tileset_dock_focused() || is_property_dock_focused() || is_log_dock_focused();
}

}  // namespace tactile::ui
