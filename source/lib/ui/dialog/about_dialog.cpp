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

#include "core/predef.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {
namespace {

inline constinit bool show_dialog = false;

}  // namespace

void open_about_dialog()
{
  show_dialog = true;
}

void update_about_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.about_tactile.c_str(),
      .accept_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (show_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    show_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted("Tactile " TACTILE_VERSION_STRING
                           " (C) Albin Johansson 2020-2022");

    ImGui::Separator();
    ImGui::TextUnformatted(lang.misc.license_info.c_str());

    ImGui::Spacing();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(lang.misc.repository_link.c_str());

    ImGui::SameLine();
    if (ui_button(TAC_ICON_LINK, lang.tooltip.repository_link.c_str())) {
      cen::open_url("https://www.github.com/albin-johansson/tactile");
    }

    ImGui::Spacing();
    ImGui::TextUnformatted(lang.misc.font_awesome_credit.c_str());
  }
}

}  // namespace tactile::ui
