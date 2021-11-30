#include "help_menu.hpp"

#include <centurion.hpp>  // open_url
#include <imgui.h>

#include "editor/gui/icons.hpp"
#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/dialogs/about_dialog.hpp"
#include "editor/gui/widgets/dialogs/credits_dialog.hpp"

namespace Tactile {
namespace {

constinit bool gShowAboutTactile = false;
constinit bool gShowAboutImgui = false;
constinit bool gShowCredits = false;

}  // namespace

void UpdateHelpMenu()
{
  if (ImGui::BeginMenu("Help")) {
    gShowAboutTactile = ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...");
    gShowAboutImgui = ImGui::MenuItem(TAC_ICON_ABOUT " About Dear ImGui...");

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_BUG " Report Issue...")) {
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    gShowCredits = ImGui::MenuItem("Credits...");

    ImGui::EndMenu();
  }
}

void UpdateHelpMenuWindows()
{
  if (gShowAboutTactile) {
    UpdateAboutDialog(&gShowAboutTactile);
  }

  if (gShowAboutImgui) {
    CenterNextWindowOnAppearance();
    ImGui::ShowAboutWindow(&gShowAboutImgui);
  }

  if (gShowCredits) {
    OpenCreditsDialog();
    gShowCredits = false;
  }

  UpdateCreditsDialog();
}

}  // namespace Tactile
