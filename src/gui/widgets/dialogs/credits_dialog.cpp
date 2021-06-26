#include "credits_dialog.hpp"

#include "aliases/czstring.hpp"
#include "imgui.h"

namespace tactile {
namespace {

inline constexpr auto table_flags =
    ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
    ImGuiTableFlags_Resizable | ImGuiTableFlags_SizingStretchProp;

void Row(const czstring lib, const czstring version, const czstring license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::Text("%s", lib);

  ImGui::TableNextColumn();
  ImGui::Text("%s", version);

  ImGui::TableNextColumn();
  ImGui::Text("%s", license);
}

}  // namespace

void UpdateCreditsDialog(bool* open)
{
  constexpr auto flags =
      ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
  if (ImGui::Begin("Credits", open, flags))
  {
    ImGui::Text(
        "Tactile is developed using the following open-source libraries.");
    ImGui::Spacing();

    if (ImGui::BeginTable("##CreditsTable", 3, table_flags))
    {
      ImGui::TableSetupColumn("Library");
      ImGui::TableSetupColumn("Version");
      ImGui::TableSetupColumn("License");
      ImGui::TableHeadersRow();

      Row("Centurion", "6.1.0", "MIT");
      Row("EnTT", "3.7.1", "MIT");
      Row("GLEW", "2.1.0", "BSD/MIT");
      Row("IconFontCppHeaders", "N/A", "Zlib");
      Row("Dear ImGui", "1.84", "MIT");
      Row("JSON for Modern C++", "3.9.1", "MIT");
      Row("Nenya", "0.1.0", "MIT");
      Row("Rune", "0.1.0", "MIT");

      ImGui::EndTable();
    }
  }

  ImGui::End();
}

}  // namespace tactile
