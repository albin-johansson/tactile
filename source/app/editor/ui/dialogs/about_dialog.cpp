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

#include "about_dialog.hpp"

#include <centurion/system.hpp>
#include <imgui.h>

#include "editor/ui/common/button.hpp"
#include "editor/ui/icons.hpp"
#include "meta/build.hpp"

#define TAC_URL "https://www.github.com/albin-johansson/tactile"
#define TAC_LICENSE "GPL v3.0"

namespace tactile::ui {

AboutDialog::AboutDialog() : ADialog{"About Tactile"}
{
  set_accept_button_label(nullptr);
  set_close_button_label("Close");
}

void AboutDialog::show()
{
  make_visible();
}

void AboutDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  ImGui::TextUnformatted("Tactile " TACTILE_VERSION_STRING
                         " (C) Albin Johansson 2020-2022");
  ImGui::Separator();

  ImGui::TextUnformatted(ICON_FA_SCALE_BALANCED
                         " This tool is open-source software, using the " TAC_LICENSE
                         " license.");

  ImGui::Spacing();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(ICON_FA_CODE " Source: " TAC_URL);
  ImGui::SameLine();
  if (button(TAC_ICON_LINK, "Open the GitHub repository in your browser")) {
    cen::open_url(TAC_URL);
  }

  ImGui::Spacing();

  ImGui::TextUnformatted(ICON_FA_FONT_AWESOME " Icons by Font Awesome.");
}

}  // namespace tactile::ui
