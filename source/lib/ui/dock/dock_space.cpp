// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "dock_space.hpp"

#include <imgui_internal.h>

#include "io/directories.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/settings.hpp"
#include "tactile/base/container/maybe.hpp"
#include "tactile/base/container/path.hpp"

namespace tactile::ui {
namespace {

inline constinit Maybe<ImGuiID> gDockRootId;

}  // namespace

void update_dock_space()
{
  static bool initialized = false;

  gDockRootId = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
  if (!initialized) {
    const auto size = ImGui::GetMainViewport()->Size;
    if (size.x > 0 && size.y > 0) {
      const auto& settings = get_settings();

      if (!settings.test_flag(SETTINGS_RESTORE_LAYOUT_BIT) ||
          !fs::exists(get_widget_ini_path())) {
        load_default_layout(gDockRootId.value(), false);
      }

      initialized = true;
    }
  }
}

void load_default_layout(ImGuiID id, const bool reset_visibility)
{
  ImGui::DockBuilderRemoveNodeChildNodes(id);

  const auto& lang = get_current_language();
  const auto root = id;

  auto right = ImGui::DockBuilderSplitNode(id, ImGuiDir_Right, 0.33f, nullptr, &id);
  const auto right_bottom =
      ImGui::DockBuilderSplitNode(right, ImGuiDir_Down, 0.40f, nullptr, &right);

  const auto bottom = ImGui::DockBuilderSplitNode(id, ImGuiDir_Down, 0.25f, nullptr, &id);

  ImGui::DockBuilderDockWindow("Viewport", root);
  ImGui::DockBuilderDockWindow(lang.window.tileset_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.property_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.component_dock.c_str(), right);
  ImGui::DockBuilderDockWindow(lang.window.animation_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.layer_dock.c_str(), right_bottom);
  ImGui::DockBuilderDockWindow(lang.window.log_dock.c_str(), bottom);

  ImGui::DockBuilderFinish(id);

  if (reset_visibility) {
    get_settings().reset_dock_visibilities();
  }
}

void reset_layout()
{
  load_default_layout(gDockRootId.value(), true);
}

}  // namespace tactile::ui
