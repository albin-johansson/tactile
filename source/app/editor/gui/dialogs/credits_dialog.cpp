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

#include "editor/gui/scoped.hpp"
#include "tactile.hpp"

namespace tactile {
namespace {

constexpr auto _table_flags = ImGuiTableFlags_RowBg |      //
                              ImGuiTableFlags_Borders |    //
                              ImGuiTableFlags_Resizable |  //
                              ImGuiTableFlags_SizingStretchProp;

void _row(const char* lib, const char* license)
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
  set_accept_button_label(nullptr);
  set_close_button_label("Close");
}

void CreditsDialog::show()
{
  make_visible();
}

void CreditsDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  ImGui::TextUnformatted(
      "Tactile is developed using the following open-source libraries.");
  ImGui::Spacing();

  if (scoped::Table table{"##CreditsTable", 2, _table_flags}; table.is_open()) {
    ImGui::TableSetupColumn("Library");
    ImGui::TableSetupColumn("License");
    ImGui::TableHeadersRow();

    _row("Boost", "BSL-1.0");
    _row("Centurion", "MIT");
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
  }
}

}  // namespace tactile
