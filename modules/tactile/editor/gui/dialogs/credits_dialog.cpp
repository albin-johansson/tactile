#include "credits_dialog.hpp"

#include <imgui.h>
#include <tactile_def.hpp>

#include "editor/gui/alignment.hpp"
#include "editor/gui/scoped.hpp"

namespace Tactile {
namespace {

constexpr auto gWindowFlags =
    ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;

constexpr auto gTableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                             ImGuiTableFlags_Resizable |
                             ImGuiTableFlags_SizingStretchProp;

constinit bool gIsVisible = false;

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
  if (Scoped::Modal modal{"Credits", gWindowFlags, &gIsVisible}; modal.IsOpen()) {
    ImGui::TextUnformatted(
        "Tactile is developed using the following open-source libraries.");
    ImGui::Spacing();

    if (Scoped::Table table{"##CreditsTable", 2, gTableFlags}; table.IsOpen()) {
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
      Row("nativefiledialog", "Zlib");
      Row("Protocol Buffers", "BSD");
      Row("pugixml", "MIT");
      Row("SDL2", "Zlib");
      Row("SDL2_image", "Zlib");
      Row("stb_image", "MIT");
      Row("yaml-cpp", "MIT");
      Row("googletest", "BSD");
    }
  }
}

void OpenCreditsDialog()
{
  gIsVisible = true;
  ImGui::OpenPopup("Credits");
}

}  // namespace Tactile
