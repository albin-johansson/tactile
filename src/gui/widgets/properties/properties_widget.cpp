#include "properties_widget.hpp"

#include <IconsFontAwesome5.h>

#include "gui/widgets/button_ex.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline constexpr auto table_flags =
    ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable;

}  // namespace

void UpdatePropertiesWidget()
{
  if (ImGui::Begin("Properties"))
  {
    if (ButtonEx(ICON_FA_PLUS_CIRCLE, "Add property."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_MINUS_CIRCLE, "Remove property."))
    {}

    ImGui::SameLine();
    if (ButtonEx(ICON_FA_EDIT, "Rename property."))
    {}

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
  }

  ImGui::End();
}

}  // namespace tactile
