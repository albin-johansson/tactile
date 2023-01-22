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

#include <centurion/system.hpp>
#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/scoped.hpp"

namespace tactile::ui {
namespace {

inline constinit bool gOpenDialog = false;

void ui_dependency_row(const char* lib, const char* license, const char* license_url)
{
  ImGui::TableNextRow();

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(lib);

  ImGui::TableNextColumn();
  ImGui::TextUnformatted(license);

  if (ImGui::IsItemHovered()) {
    ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);

    if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
      cen::open_url(license_url);
    }
  }
}

}  // namespace

void open_credits_dialog()
{
  gOpenDialog = true;
}

void update_credits_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.credits.c_str(),
      .close_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (gOpenDialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gOpenDialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.credits_info.c_str());
    ImGui::Spacing();

    const auto table_flags = ImGuiTableFlags_RowBg |      //
                             ImGuiTableFlags_Borders |    //
                             ImGuiTableFlags_Resizable |  //
                             ImGuiTableFlags_SizingStretchProp;

    if (const Table table {"##CreditsTable", 2, table_flags}; table.is_open()) {
      ImGui::TableSetupColumn(lang.misc.library.c_str());
      ImGui::TableSetupColumn(lang.misc.license.c_str());
      ImGui::TableHeadersRow();

      ui_dependency_row("argparse",
                        "MIT",
                        "https://github.com/p-ranav/argparse/blob/master/LICENSE");
      ui_dependency_row("Boost",
                        "BSL-1.0",
                        "https://github.com/boostorg/boost/blob/master/LICENSE_1_0.txt");
      ui_dependency_row("Centurion",
                        "MIT",
                        "https://github.com/albin-johansson/centurion/blob/dev/LICENSE");
      ui_dependency_row("cppcodec",
                        "MIT",
                        "https://github.com/tplgy/cppcodec/blob/master/LICENSE");
      ui_dependency_row("Dear ImGui",
                        "MIT",
                        "https://github.com/ocornut/imgui/blob/master/LICENSE.txt");
      ui_dependency_row("doctest",
                        "MIT",
                        "https://github.com/doctest/doctest/blob/master/LICENSE.txt");
      ui_dependency_row("EnTT",
                        "MIT",
                        "https://github.com/skypjack/entt/blob/master/LICENSE");
      ui_dependency_row("fmt",
                        "MIT",
                        "https://github.com/fmtlib/fmt/blob/master/LICENSE.rst");
      ui_dependency_row("GLEW",
                        "BSD/MIT",
                        "https://github.com/nigels-com/glew/blob/master/LICENSE.txt");
      ui_dependency_row("glm",
                        "Happy Bunny/MIT",
                        "https://github.com/g-truc/glm/blob/master/copying.txt");
      ui_dependency_row(
          "IconFontCppHeaders",
          "Zlib",
          "https://github.com/juliettef/IconFontCppHeaders/blob/main/licence.txt");
      ui_dependency_row("JSON for Modern C++",
                        "MIT",
                        "https://github.com/nlohmann/json/blob/develop/LICENSE.MIT");
      ui_dependency_row("Magic Enum C++",
                        "MIT",
                        "https://github.com/Neargye/magic_enum/blob/master/LICENSE");
      ui_dependency_row("Protocol Buffers",
                        "BSD",
                        "https://github.com/protocolbuffers/protobuf/blob/main/LICENSE");
      ui_dependency_row("pugixml",
                        "MIT",
                        "https://github.com/zeux/pugixml/blob/master/LICENSE.md");
      ui_dependency_row("SDL2",
                        "Zlib",
                        "https://github.com/libsdl-org/SDL/blob/main/LICENSE.txt");
      ui_dependency_row("SDL2_image",
                        "Zlib",
                        "https://github.com/libsdl-org/SDL_image/blob/main/LICENSE.txt");
      ui_dependency_row("spdlog",
                        "MIT",
                        "https://github.com/gabime/spdlog/blob/v1.x/LICENSE");
      ui_dependency_row("stb_image",
                        "MIT",
                        "https://github.com/nothings/stb/blob/master/LICENSE");
      ui_dependency_row("tinyfiledialogs",
                        "Zlib",
                        "https://sourceforge.net/projects/tinyfiledialogs/");
      ui_dependency_row("tl-expected",
                        "CC0-1.0",
                        "https://github.com/TartanLlama/expected/blob/master/COPYING");
      ui_dependency_row("yaml-cpp",
                        "MIT",
                        "https://github.com/jbeder/yaml-cpp/blob/master/LICENSE");
      ui_dependency_row("Zlib",
                        "Zlib",
                        "https://github.com/madler/zlib/blob/master/LICENSE");
      ui_dependency_row("zstd",
                        "BSD/GPLv2",
                        "https://github.com/facebook/zstd/blob/dev/LICENSE");
      ui_dependency_row("Roboto",
                        "Apache 2.0",
                        "https://github.com/googlefonts/roboto/blob/main/LICENSE");
    }
  }
}

}  // namespace tactile::ui
