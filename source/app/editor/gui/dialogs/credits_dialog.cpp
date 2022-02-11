#include "credits_dialog.hpp"

#include <imgui.h>

#include "editor/gui/scoped.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constexpr auto _table_flags = ImGuiTableFlags_RowBg |      //
                              ImGuiTableFlags_Borders |    //
                              ImGuiTableFlags_Resizable |  //
                              ImGuiTableFlags_SizingStretchProp;

void _row(const c_str lib, const c_str license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(license);
}

}  // namespace

credits_dialog::credits_dialog() : dialog_base{"Credits"}
{
  set_accept_button_label(nullptr);
  set_close_button_label("Close");
}

void credits_dialog::show()
{
  make_visible();
}

void credits_dialog::on_update(const document_model&, entt::dispatcher&)
{
  ImGui::TextUnformatted(
      "Tactile is developed using the following open-source libraries.");
  ImGui::Spacing();

  if (scoped::table table{"##CreditsTable", 2, _table_flags}; table.is_open()) {
    ImGui::TableSetupColumn("Library");
    ImGui::TableSetupColumn("License");
    ImGui::TableHeadersRow();

    _row("Centurion", "MIT");
    _row("cpp-codec", "MIT");
    _row("Dear ImGui", "MIT");
    _row("EnTT", "MIT");
    _row("fmt", "MIT");
    _row("GLEW", "BSD/MIT");
    _row("IconFontCppHeaders", "Zlib");
    _row("JSON for Modern C++", "MIT");
    _row("Magic Enum C++", "MIT");
    _row("tinyfiledialogs", "Zlib");
    _row("Protocol Buffers", "BSD");
    _row("pugixml", "MIT");
    _row("SDL2", "Zlib");
    _row("SDL2_image", "Zlib");
    _row("stb_image", "MIT");
    _row("yaml-cpp", "MIT");
    _row("googletest", "BSD");
    _row("Zlib", "Zlib");
  }
}

}  // namespace tactile
