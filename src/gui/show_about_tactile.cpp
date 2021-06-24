#include "show_about_tactile.hpp"

#include "imgui.h"

namespace tactile {

void ShowAboutTactile(bool* open)
{
  ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2{200, 150});

  if (ImGui::Begin("About Tactile", open, ImGuiWindowFlags_AlwaysAutoResize))
  {
    ImGui::Text("Tactile 0.2.0");
    ImGui::Separator();

    ImGui::Text(
        "This tool is open-source software, using under the GPL v3.0 license.");
    ImGui::Text("Source code: https://www.github.com/albin-johansson/tactile");

    ImGui::Spacing();
    ImGui::Text("(C) Albin Johansson 2020-2021");
  }
  ImGui::End();

  ImGui::PopStyleVar();
}

}  // namespace tactile
