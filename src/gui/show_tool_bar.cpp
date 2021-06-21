#include "show_tool_bar.hpp"

#include "imgui.h"

namespace tactile {

void ShowToolBar()
{
  if (ImGui::Begin("Toolbar"))
  {
    if (ImGui::Button("New"))
    {}

    ImGui::SameLine();
    if (ImGui::Button("Open"))
    {}

    ImGui::SameLine();
    ImGui::Spacing();

    ImGui::SameLine();

    if (ImGui::Button("Undo"))
    {}

    ImGui::SameLine();
    if (ImGui::Button("Redo"))
    {}

    ImGui::SameLine();
    ImGui::Spacing();
  }
  ImGui::End();
}

}  // namespace tactile
