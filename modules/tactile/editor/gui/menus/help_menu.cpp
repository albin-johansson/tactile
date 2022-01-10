#include "help_menu.hpp"

#include <centurion.hpp>  // open_url
#include <imgui.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/dialogs/about_dialog.hpp"
#include "editor/gui/dialogs/credits_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {

void HelpMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (Scoped::Menu menu{"Help"}; menu.IsOpen()) {
    mShowAboutTactile = ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...");
    mShowAboutImGui = ImGui::MenuItem(TAC_ICON_ABOUT " About Dear ImGui...");

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_BUG " Report Issue...")) {
      cen::OpenURL("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Credits...")) {
      mCreditsDialog.Open();
    }
  }

  mCreditsDialog.Update(model, dispatcher);
}

void HelpMenu::UpdateWindows()
{
  if (mShowAboutTactile) {
    UpdateAboutDialog(&mShowAboutTactile);
  }

  if (mShowAboutImGui) {
    CenterNextWindowOnAppearance();
    ImGui::ShowAboutWindow(&mShowAboutImGui);
  }
}

}  // namespace Tactile
