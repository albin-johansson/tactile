#include "debug_menu.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/menu.hpp"
#include "editor/gui/widgets/common/window.hpp"

namespace Tactile {
namespace {

constinit bool show_metrics = false;
constinit bool show_demo = false;
constinit bool show_style_editor = false;

}  // namespace

void UpdateDebugMenu()
{
  if (auto menu = Menu{"Debug"}) {
    show_metrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (cen::is_debug_build()) {
      ImGui::Separator();
      show_demo = ImGui::MenuItem("Show Demo Window...");
      show_style_editor = ImGui::MenuItem("Show Style Editor...");
    }
  }
}

void UpdateDebugMenuWindows()
{
  if (show_metrics) {
    CenterNextWindowOnAppearance();
    ImGui::ShowMetricsWindow(&show_metrics);
  }

  if constexpr (cen::is_debug_build()) {
    if (show_demo) {
      ImGui::ShowDemoWindow(&show_demo);
    }

    if (show_style_editor) {
      auto editor = Window{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace Tactile
