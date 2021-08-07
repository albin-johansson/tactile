#include "help_menu.hpp"

#include <imgui.h>

#include <centurion.hpp>  // is_debug_build

#include "gui/icons.hpp"
#include "gui/widgets/dialogs/about_dialog.hpp"
#include "gui/widgets/dialogs/credits_dialog.hpp"

namespace Tactile {
namespace {

constinit bool show_about_tactile = false;
constinit bool show_about_imgui = false;
constinit bool show_credits = false;
constinit bool show_metrics = false;
constinit bool show_demo = false;
constinit bool show_style_editor = false;

}  // namespace

void UpdateHelpMenu()
{
  if (ImGui::BeginMenu("Help"))
  {
    show_about_tactile = ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...");
    show_about_imgui = ImGui::MenuItem(TAC_ICON_ABOUT " About Dear ImGui...");
    show_credits = ImGui::MenuItem("Credits...");

    ImGui::Separator();

    show_metrics = ImGui::MenuItem(TAC_ICON_METRICS " Show metrics...");

    if constexpr (cen::is_debug_build())
    {
      ImGui::Separator();
      show_demo = ImGui::MenuItem("Show demo window...");
      show_style_editor = ImGui::MenuItem("Show style editor...");
    }

    ImGui::EndMenu();
  }
}

void UpdateHelpMenuWindows()
{
  if (show_about_tactile)
  {
    UpdateAboutDialog(&show_about_tactile);
  }

  if (show_about_imgui)
  {
    ImGui::ShowAboutWindow(&show_about_imgui);
  }

  if (show_credits)
  {
    UpdateCreditsDialog(&show_credits);
  }

  if (show_metrics)
  {
    ImGui::ShowMetricsWindow(&show_metrics);
  }

  if constexpr (cen::is_debug_build())
  {
    if (show_demo)
    {
      ImGui::ShowDemoWindow(&show_demo);
    }

    if (show_style_editor)
    {
      if (ImGui::Begin("Style editor"))
      {
        ImGui::ShowStyleEditor();
      }
      ImGui::End();
    }
  }
}

}  // namespace Tactile
