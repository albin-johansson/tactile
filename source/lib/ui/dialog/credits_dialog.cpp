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

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

inline constinit bool show_dialog = false;

void ui_dependency_row(const char* lib, const char* license)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(license);
}

}  // namespace

void open_credits_dialog()
{
  show_dialog = true;
}

void update_credits_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.credits.c_str(),
      .close_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (show_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    show_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.credits_info.c_str());
    ImGui::Spacing();

    constexpr auto table_flags = ImGuiTableFlags_RowBg |      //
                                 ImGuiTableFlags_Borders |    //
                                 ImGuiTableFlags_Resizable |  //
                                 ImGuiTableFlags_SizingStretchProp;

    if (const Table table {"##CreditsTable", 2, table_flags}; table.is_open()) {
      ImGui::TableSetupColumn(lang.misc.library.c_str());
      ImGui::TableSetupColumn(lang.misc.license.c_str());
      ImGui::TableHeadersRow();

      ui_dependency_row("argparse", "MIT");
      ui_dependency_row("Boost", "BSL-1.0");
      ui_dependency_row("Centurion", "MIT");
      ui_dependency_row("cppcodec", "MIT");
      ui_dependency_row("Dear ImGui", "MIT");
      ui_dependency_row("doctest", "MIT");
      ui_dependency_row("EnTT", "MIT");
      ui_dependency_row("fmt", "MIT");
      ui_dependency_row("GLEW", "BSD/MIT");
      ui_dependency_row("glm", "Happy Bunny/MIT");  // Yes, there is a Happy Bunny license
      ui_dependency_row("IconFontCppHeaders", "Zlib");
      ui_dependency_row("JSON for Modern C++", "MIT");
      ui_dependency_row("Magic Enum C++", "MIT");
      ui_dependency_row("Protocol Buffers", "BSD");
      ui_dependency_row("pugixml", "MIT");
      ui_dependency_row("SDL2", "Zlib");
      ui_dependency_row("SDL2_image", "Zlib");
      ui_dependency_row("spdlog", "MIT");
      ui_dependency_row("stb_image", "MIT");
      ui_dependency_row("tinyfiledialogs", "Zlib");
      ui_dependency_row("tl-expected", "CC0-1.0");
      ui_dependency_row("yaml-cpp", "MIT");
      ui_dependency_row("Zlib", "Zlib");
      ui_dependency_row("zstd", "BSD/GPLv2");
    }
  }
}

}  // namespace tactile::ui
