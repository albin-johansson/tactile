// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "debug_menu.hpp"

#include <imgui.h>

#include "io/directories.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "tactile/base/prelude.hpp"
#include "ui/style/alignment.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

struct DebugMenuState final {
  bool show_metrics {};
  bool show_demo {};
  bool show_style_editor {};
};

inline constinit DebugMenuState gMenuState;

}  // namespace

void update_debug_menu()
{
  const auto& lang = get_current_language();

  if (const Menu menu {lang.menu.debug.c_str()}; menu.is_open()) {
    gMenuState.show_metrics = ImGui::MenuItem(lang.action.show_metrics.c_str());

    if constexpr (kIsDebugBuild) {
      ImGui::Separator();
      gMenuState.show_demo = ImGui::MenuItem("Show Demo Window...");
      gMenuState.show_style_editor = ImGui::MenuItem("Show Style Editor...");
    }

    ImGui::Separator();

    if (ImGui::MenuItem(lang.action.open_persistent_file_dir.c_str())) {
      open_directory(get_persistent_file_dir());
    }
  }

  if (gMenuState.show_metrics) {
    center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&gMenuState.show_metrics);
  }

  if (gMenuState.show_demo) {
    ImGui::ShowDemoWindow(&gMenuState.show_demo);
  }

  if (gMenuState.show_style_editor) {
    const Window editor {"Style Editor"};
    ImGui::ShowStyleEditor();
  }
}

}  // namespace tactile::ui
