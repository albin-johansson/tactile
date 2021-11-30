#include "debug_menu.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/menu.hpp"
#include "editor/gui/widgets/common/window.hpp"

namespace Tactile {
namespace {

constinit bool gShowMetrics = false;
constinit bool gShowDemo = false;
constinit bool gShowStyleEditor = false;

}  // namespace

void UpdateDebugMenu()
{
  if (auto menu = Menu{"Debug"}) {
    gShowMetrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (cen::is_debug_build()) {
      ImGui::Separator();
      gShowDemo = ImGui::MenuItem("Show Demo Window...");
      gShowStyleEditor = ImGui::MenuItem("Show Style Editor...");
    }
  }
}

void UpdateDebugMenuWindows()
{
  if (gShowMetrics) {
    CenterNextWindowOnAppearance();
    ImGui::ShowMetricsWindow(&gShowMetrics);
  }

  if constexpr (cen::is_debug_build()) {
    if (gShowDemo) {
      ImGui::ShowDemoWindow(&gShowDemo);
    }

    if (gShowStyleEditor) {
      auto editor = Window{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace Tactile
