#include "about_dialog.hpp"

#include <IconsFontAwesome5.h>
#include <imgui.h>

namespace Tactile {

void UpdateAboutDialog(bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{200, 150});

  if (ImGui::Begin(ICON_FA_QUESTION_CIRCLE " About Tactile",
                   open,
                   ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Tactile 0.2.0 (C) Albin Johansson 2020-2021");
    ImGui::Separator();

    ImGui::Text(
        "This tool is open-source software, using the GPL v3.0 license.");
    ImGui::Text("Source code: https://www.github.com/albin-johansson/tactile");

    ImGui::Spacing();
    ImGui::Text("Icons by Font Awesome.");
  }
  ImGui::End();

  ImGui::PopStyleVar();
}

}  // namespace Tactile
