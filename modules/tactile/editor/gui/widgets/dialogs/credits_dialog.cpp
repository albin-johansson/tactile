#include "credits_dialog.hpp"

#include <imgui.h>

#include <tactile-base/tactile_std.hpp>

#include "editor/gui/widgets/alignment.hpp"
#include "editor/gui/widgets/common/modal.hpp"

namespace Tactile {
namespace {

constexpr auto window_flags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constexpr auto table_flags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                             ImGuiTableFlags_Resizable |
                             ImGuiTableFlags_SizingStretchProp;

constinit bool is_visible = false;

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

void UpdateCreditsDialog()
{
  CenterNextWindowOnAppearance();
  if (auto modal = Modal{"Credits", window_flags, &is_visible}) {
    ImGui::TextUnformatted(
        "Tactile is developed using the following open-source libraries.");
    ImGui::Spacing();

    if (ImGui::BeginTable("##CreditsTable", 3, table_flags)) {
      ImGui::TableSetupColumn("Library");
      ImGui::TableSetupColumn("Version");
      ImGui::TableSetupColumn("License");
      ImGui::TableHeadersRow();

      Row("Centurion", "6.3.0", "MIT");
      Row("Dear ImGui", "1.84.1", "MIT");
      Row("EnTT", "3.8.1", "MIT");
      Row("fmt", "8.1.0", "MIT");
      Row("GLEW", "2.2.0", "BSD/MIT");
      Row("IconFontCppHeaders", "N/A", "Zlib");
      Row("JSON for Modern C++", "3.9.1", "MIT");
      Row("Magic Enum C++", "0.7.3", "MIT");
      Row("portable-file-dialogs", "0.1.0", "WTFPL");
      Row("pugixml", "1.11.4", "MIT");
      Row("SDL", "2.0.16", "Zlib");
      Row("SDL_image", "2.0.5", "Zlib");
      Row("SDL_ttf", "2.0.15", "Zlib");
      Row("stb_image", "2.26", "MIT");
      Row("yaml-cpp", "0.7.0", "MIT");

      ImGui::EndTable();
    }
  }
}

void OpenCreditsDialog()
{
  is_visible = true;
  ImGui::OpenPopup("Credits");
}

}  // namespace Tactile
