#include "debug_menu.hpp"

#include <centurion.hpp>
#include <imgui.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"
#include "meta/build.hpp"

namespace tactile {

void DebugMenu::Update()
{
  if (scoped::Menu menu{"Debug"}; menu.IsOpen()) {
    mShowMetrics = ImGui::MenuItem(TAC_ICON_METRICS " Show Metrics...");

    if constexpr (is_debug_build) {
      ImGui::Separator();
      mShowDemo = ImGui::MenuItem("Show Demo Window...");
      mShowStyleEditor = ImGui::MenuItem("Show Style Editor...");
    }
  }
}

void DebugMenu::UpdateWindows()
{
  if (mShowMetrics) {
    center_next_window_on_appearance();
    ImGui::ShowMetricsWindow(&mShowMetrics);
  }

  if constexpr (is_debug_build) {
    if (mShowDemo) {
      ImGui::ShowDemoWindow(&mShowDemo);
    }

    if (mShowStyleEditor) {
      scoped::Window editor{"Style Editor"};
      ImGui::ShowStyleEditor();
    }
  }
}

}  // namespace tactile
