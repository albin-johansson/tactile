#include "show_properties.hpp"

#include "imgui.h"

namespace tactile {
namespace {

inline constexpr auto table_flags =
    ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable;

}  // namespace

void ShowProperties()
{
  if (ImGui::Begin("Properties"))
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});

    ImGui::Button("New");

    ImGui::SameLine();
    ImGui::Button("Remove");

    ImGui::SameLine();
    ImGui::Button("Rename");

    ImGui::Separator();

    if (ImGui::BeginTable("PropertiesTable", 2, table_flags))
    {
      ImGui::TableSetupColumn("Name");
      ImGui::TableSetupColumn("Value");
      ImGui::TableHeadersRow();

      for (int i = 0; i < 4; ++i)
      {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Foo");

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("Bar");
      }

      ImGui::EndTable();
    }

    ImGui::PopStyleVar();
  }

  ImGui::End();
}

}  // namespace tactile
