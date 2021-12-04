#include "debug_menu.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "build.hpp"
#include "editor/gui/alignment.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

void DebugMenu::Update()
{
  if (Scoped::Menu menu{"Debug"}; menu.IsOpen()) {
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
      Scoped::Window editor{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace Tactile
