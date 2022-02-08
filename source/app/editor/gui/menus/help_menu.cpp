#include "help_menu.hpp"

#include <centurion.hpp>  // open_url
#include <imgui.h>

#include "editor/gui/alignment.hpp"
#include "editor/gui/dialogs/about_dialog.hpp"
#include "editor/gui/dialogs/credits_dialog.hpp"
#include "editor/gui/icons.hpp"
#include "editor/gui/scoped.hpp"

namespace tactile {

void HelpMenu::Update(const Model& model, entt::dispatcher& dispatcher)
{
  if (scoped::menu menu{"Help"}; menu.is_open()) {
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

  mAboutDialog.update(model, dispatcher);
  mCreditsDialog.update(model, dispatcher);
}

void HelpMenu::UpdateWindows()
{
  if (mShowAboutImGui) {
    center_next_window_on_appearance();
    ImGui::ShowAboutWindow(&mShowAboutImGui);
  }
}

}  // namespace tactile
