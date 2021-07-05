#include "about_dialog.hpp"

#include <imgui.h>

#include <centurion.hpp>  // open_url

#include "gui/icons.hpp"
#include "gui/widgets/common/button_ex.hpp"

namespace Tactile {

void UpdateAboutDialog(bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{200, 150});

  if (ImGui::Begin(TAC_ICON_ABOUT " About Tactile",
                   open,
                   ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Tactile 0.2.0 (C) Albin Johansson 2020-2021");
    ImGui::Separator();

    ImGui::Text("This tool is open-source software, using the GPL v3.0 license.");

    ImGui::AlignTextToFramePadding();
    ImGui::Text("Source code: https://www.github.com/albin-johansson/tactile");
    ImGui::SameLine();
    if (ButtonEx(TAC_ICON_LINK, "Open the GitHub repository in your browser."))
    {
      cen::open_url("https://www.github.com/albin-johansson/tactile");
    }

    ImGui::Spacing();
    ImGui::Text("Icons by Font Awesome.");
  }
  ImGui::End();

  ImGui::PopStyleVar();
}

}  // namespace Tactile
