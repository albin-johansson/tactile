/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "model/systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/scoped.hpp"

#define GITHUB_URL "https://github.com/"
#define SOURCEFORGE_URL "https://sourceforge.net/"

namespace tactile::ui {
namespace {

void _push_row(const char* lib, const char* license, const char* license_url)
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

void push_credits_dialog(const Model& model, CreditsDialogState& state)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions dialog_options {
      .title = strings.window.credits.c_str(),
      .close_label = strings.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (const ScopedDialog dialog {dialog_options}; dialog.was_opened()) {
    ImGui::TextUnformatted(strings.misc.credits_info.c_str());
    ImGui::Spacing();

    const auto table_flags = ImGuiTableFlags_RowBg |      //
                             ImGuiTableFlags_Borders |    //
                             ImGuiTableFlags_Resizable |  //
                             ImGuiTableFlags_SizingStretchProp;

    if (const Table table {"##CreditsTable", 2, table_flags}; table.is_open()) {
      ImGui::TableSetupColumn(strings.misc.library.c_str());
      ImGui::TableSetupColumn(strings.misc.license.c_str());
      ImGui::TableHeadersRow();

      _push_row("argparse", "MIT", GITHUB_URL "p-ranav/argparse/blob/master/LICENSE");
      _push_row("Boost",
                "BSL-1.0",
                GITHUB_URL "boostorg/boost/blob/master/LICENSE_1_0.txt");
      _push_row("Centurion",
                "MIT",
                GITHUB_URL "albin-johansson/centurion/blob/dev/LICENSE");
      _push_row("cppcodec", "MIT", GITHUB_URL "tplgy/cppcodec/blob/master/LICENSE");
      _push_row("Dear ImGui", "MIT", GITHUB_URL "ocornut/imgui/blob/master/LICENSE.txt");
      _push_row("doctest", "MIT", GITHUB_URL "doctest/doctest/blob/master/LICENSE.txt");
      _push_row("EnTT", "MIT", GITHUB_URL "skypjack/entt/blob/master/LICENSE");
      _push_row("fmt", "MIT", GITHUB_URL "fmtlib/fmt/blob/master/LICENSE.rst");
      _push_row("glad", "MIT", GITHUB_URL "Dav1dde/glad/blob/glad2/LICENSE");
      _push_row("glm",
                "Happy Bunny/MIT",
                GITHUB_URL "g-truc/glm/blob/master/copying.txt");
      _push_row("IconFontCppHeaders",
                "Zlib",
                GITHUB_URL "juliettef/IconFontCppHeaders/blob/main/licence.txt");
      _push_row("JSON for Modern C++",
                "MIT",
                GITHUB_URL "nlohmann/json/blob/develop/LICENSE.MIT");
      _push_row("Magic Enum C++",
                "MIT",
                GITHUB_URL "Neargye/magic_enum/blob/master/LICENSE");
      _push_row("Protocol Buffers",
                "BSD",
                GITHUB_URL "protocolbuffers/protobuf/blob/main/LICENSE");
      _push_row("pugixml", "MIT", GITHUB_URL "zeux/pugixml/blob/master/LICENSE.md");
      _push_row("SDL2", "Zlib", GITHUB_URL "libsdl-org/SDL/blob/main/LICENSE.txt");
      _push_row("SDL2_image",
                "Zlib",
                GITHUB_URL "libsdl-org/SDL_image/blob/main/LICENSE.txt");
      _push_row("spdlog", "MIT", GITHUB_URL "gabime/spdlog/blob/v1.x/LICENSE");
      _push_row("stb_image", "MIT", GITHUB_URL "nothings/stb/blob/master/LICENSE");
      _push_row("Vulkan Memory Allocator",
                "MIT",
                GITHUB_URL
                "GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator/blob/master/LICENSE.txt");
      _push_row("tinyfiledialogs", "Zlib", SOURCEFORGE_URL "projects/tinyfiledialogs/");
      _push_row("tl-expected",
                "CC0-1.0",
                GITHUB_URL "TartanLlama/expected/blob/master/COPYING");
      _push_row("yaml-cpp", "MIT", GITHUB_URL "jbeder/yaml-cpp/blob/master/LICENSE");
      _push_row("Zlib", "Zlib", GITHUB_URL "madler/zlib/blob/master/LICENSE");
      _push_row("zstd", "BSD/GPLv2", GITHUB_URL "facebook/zstd/blob/dev/LICENSE");
      _push_row("Roboto",
                "Apache 2.0",
                GITHUB_URL "googlefonts/roboto/blob/main/LICENSE");
    }
  }
}

}  // namespace tactile::ui
