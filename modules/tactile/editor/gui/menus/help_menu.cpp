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
    if (ImGui::MenuItem(TAC_ICON_ABOUT " About Tactile...")) {
      mAboutDialog.Open();
    }

    mShowAboutImGui = ImGui::MenuItem(TAC_ICON_ABOUT " About Dear ImGui...");

    ImGui::Separator();
    if (ImGui::MenuItem(TAC_ICON_BUG " Report Issue...")) {
      cen::open_url("https://github.com/albin-johansson/tactile/issues/new");
    }

    ImGui::Separator();
    if (ImGui::MenuItem("Credits...")) {
      mCreditsDialog.Open();
    }
  }

  mAboutDialog.Update(model, dispatcher);
  mCreditsDialog.Update(model, dispatcher);
}

void HelpMenu::UpdateWindows()
{
  if (mShowAboutImGui) {
    CenterNextWindowOnAppearance();
    ImGui::ShowAboutWindow(&mShowAboutImGui);
  }
}

}  // namespace Tactile
