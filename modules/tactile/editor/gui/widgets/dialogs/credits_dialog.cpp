#include "credits_dialog.hpp"

#include <tactile_def.hpp>

#include <imgui.h>

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

void Row(const CStr lib, const CStr license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

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

    if (ImGui::BeginTable("##CreditsTable", 2, table_flags)) {
      ImGui::TableSetupColumn("Library");
      ImGui::TableSetupColumn("License");
      ImGui::TableHeadersRow();

      Row("Centurion", "MIT");
      Row("Dear ImGui", "MIT");
      Row("EnTT", "MIT");
      Row("fmt", "MIT");
      Row("GLEW", "BSD/MIT");
      Row("IconFontCppHeaders", "Zlib");
      Row("JSON for Modern C++", "MIT");
      Row("Magic Enum C++", "MIT");
      Row("portable-file-dialogs", "WTFPL");
      Row("Protocol Buffers", "BSD");
      Row("pugixml", "MIT");
      Row("SDL", "Zlib");
      Row("stb_image", "MIT");
      Row("yaml-cpp", "MIT");
      Row("googletest", "BSD");

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
