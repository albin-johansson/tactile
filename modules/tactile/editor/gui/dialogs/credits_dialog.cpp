#include "credits_dialog.hpp"

#include <imgui.h>

#include "editor/gui/scoped.hpp"
#include "tactile_def.hpp"

namespace tactile {
namespace {

constexpr auto gTableFlags = ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders |
                             ImGuiTableFlags_Resizable |
                             ImGuiTableFlags_SizingStretchProp;

void Row(const CStr lib, const CStr license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(license);
}

}  // namespace

CreditsDialog::CreditsDialog() : ADialog{"Credits"}
{
  SetAcceptButtonLabel(nullptr);
  SetCloseButtonLabel("Close");
}

void CreditsDialog::Open()
{
  Show();
}

void CreditsDialog::UpdateContents(const Model&, entt::dispatcher&)
{
  ImGui::TextUnformatted(
      "Tactile is developed using the following open-source libraries.");
  ImGui::Spacing();

  if (scoped::Table table{"##CreditsTable", 2, gTableFlags}; table.IsOpen()) {
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

}  // namespace tactile
