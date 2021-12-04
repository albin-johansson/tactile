#include "debug_menu.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "build.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/common/menu.hpp"
#include "editor/gui/common/window.hpp"
#include "editor/gui/icons.hpp"

namespace Tactile {

void DebugMenu::Update()
{
  Menu menu{"Debug"};
  if (menu) {
    mShowMetrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (IsDebugBuild()) {
      ImGui::Separator();
      mShowDemo = ImGui::MenuItem("Show Demo Window...");
      mShowStyleEditor = ImGui::MenuItem("Show Style Editor...");
    }
  }
}

void DebugMenu::UpdateWindows()
{
  if (mShowMetrics) {
    CenterNextWindowOnAppearance();
    ImGui::ShowMetricsWindow(&mShowMetrics);
  }

  if constexpr (IsDebugBuild()) {
    if (mShowDemo) {
      ImGui::ShowDemoWindow(&mShowDemo);
    }

    if (mShowStyleEditor) {
      Window editor{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace Tactile
