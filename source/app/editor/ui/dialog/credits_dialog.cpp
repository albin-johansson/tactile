/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "credits_dialog.hpp"

#include <imgui.h>

#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/scoped.hpp"

namespace tactile::ui {
namespace {

constexpr auto table_flags = ImGuiTableFlags_RowBg |      //
                             ImGuiTableFlags_Borders |    //
                             ImGuiTableFlags_Resizable |  //
                             ImGuiTableFlags_SizingStretchProp;

void row(const char* lib, const char* license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(license);
}

}  // namespace

CreditsDialog::CreditsDialog()
    : ADialog {"Credits"}
{
  set_accept_button_label(nothing);
}

void CreditsDialog::show()
{
  const auto& lang = get_current_language();

  set_title(lang.window.credits);
  set_close_button_label(lang.misc.close);

  make_visible();
}

void CreditsDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  ImGui::TextUnformatted(lang.misc.credits_info.c_str());
  ImGui::Spacing();

  if (Table table {"##CreditsTable", 2, table_flags}; table.is_open()) {
    ImGui::TableSetupColumn(lang.misc.library.c_str());
    ImGui::TableSetupColumn(lang.misc.license.c_str());
    ImGui::TableHeadersRow();

    row("Boost", "BSL-1.0");
    row("Centurion", "MIT");
    row("cppcodec", "MIT");
    row("Dear ImGui", "MIT");
    row("EnTT", "MIT");
    row("fmt", "MIT");
    row("GLEW", "BSD/MIT");
    row("glm", "Happy Bunny/MIT");  // Yes, there is a Happy Bunny license
    row("IconFontCppHeaders", "Zlib");
    row("JSON for Modern C++", "MIT");
    row("Magic Enum C++", "MIT");
    row("Protocol Buffers", "BSD");
    row("pugixml", "MIT");
    row("SDL2", "Zlib");
    row("SDL2_image", "Zlib");
    row("spdlog", "MIT");
    row("stb_image", "MIT");
    row("tinyfiledialogs", "Zlib");
    row("yaml-cpp", "MIT");
    row("Zlib", "Zlib");
    row("zstd", "BSD/GPLv2");
    row("googletest", "BSD");
  }
}

}  // namespace tactile::ui
