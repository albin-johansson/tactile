#include "credits_dialog.hpp"

#include <imgui.h>

#include "aliases/cstr.hpp"

namespace Tactile {
namespace {

constexpr auto table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                             ImGuiTableFlags_Resizable |
                             ImGuiTableFlags_SizingStretchProp;

void Row(const CStr lib, const CStr version, const CStr license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(version);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(license);
}

}  // namespace

void UpdateCreditsDialog(bool* open)
{
  if (ImGui::Begin("Credits",
                   open,
                   ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
  {
    ImGui::TextUnformatted(
        "Tactile is developed using the following open-source libraries.");
    ImGui::Spacing();

    if (ImGui::BeginTable("##CreditsTable", 3, table_flags))
    {
      ImGui::TableSetupColumn("Library");
      ImGui::TableSetupColumn("Version");
      ImGui::TableSetupColumn("License");
      ImGui::TableHeadersRow();

      Row("Centurion", "6.2.0", "MIT");
      Row("Dear ImGui", "1.84.1", "MIT");
      Row("EnTT", "3.8.1", "MIT");
      Row("GLEW", "2.2.0", "BSD/MIT");
      Row("IconFontCppHeaders", "N/A", "Zlib");
      Row("ImGuiFileDialog", "0.5.5", "MIT");
      Row("JSON for Modern C++", "3.9.1", "MIT");
      Row("Nenya", "0.1.0", "MIT");
      Row("portable-file-dialogs", "0.1.0", "WTFPL");
      Row("pugixml", "1.11.4", "MIT");
      Row("Rune", "0.1.0", "MIT");
      Row("SDL", "2.0.14", "Zlib");
      Row("SDL_image", "2.0.5", "Zlib");
      Row("SDL_ttf", "2.0.15", "Zlib");
      Row("stb_image", "2.26", "MIT");

      ImGui::EndTable();
    }
  }

  ImGui::End();
}

}  // namespace Tactile
