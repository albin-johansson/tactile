#include "help_menu.hpp"

#include <centurion.hpp>  // open_url
#include <imgui.h>

#include "tactile/editor/gui/icons.hpp"
#include "tactile/editor/gui/widgets/alignment.hpp"
#include "tactile/editor/gui/widgets/dialogs/about_dialog.hpp"
#include "tactile/editor/gui/widgets/dialogs/credits_dialog.hpp"

namespace Tactile {
namespace {

constinit bool show_about_tactile = false;
constinit bool show_about_imgui = false;
constinit bool show_credits = false;

}  // namespace

void UpdateHelpMenu()
{
  if (ImGui::BeginMenu("Help")) {
    show_about_tactile = ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...");
    show_about_imgui = ImGui::MenuItem(TAC_ICON_ABOUT " About Dear ImGui...");

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_BUG " Report Issue...")) {
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    show_credits = ImGui::MenuItem("Credits...");

    ImGui::EndMenu();
  }
}

void UpdateHelpMenuWindows()
{
  if (show_about_tactile) {
    UpdateAboutDialog(&show_about_tactile);
  }

  if (show_about_imgui) {
    CenterNextWindowOnAppearance();
    ImGui::ShowAboutWindow(&show_about_imgui);
  }

  if (show_credits) {
    OpenCreditsDialog();
    show_credits = false;
  }

  UpdateCreditsDialog();
}

}  // namespace Tactile
