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

#include "about_dialog.hpp"

#include <centurion/system.hpp>
#include <imgui.h>

#include "common/predef.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/style/icons.hpp"
#include "ui/widget/widgets.hpp"

namespace tactile::ui {

void push_about_dialog(const Registry& registry, AboutDialogState& state)
{
  const auto& strings = sys::get_current_language_strings(registry);

  DialogOptions dialog_options {
      .title = strings.window.about_tactile.c_str(),
      .accept_label = strings.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (const ScopedDialog dialog {dialog_options}; dialog.was_opened()) {
    ImGui::TextUnformatted("Tactile " TACTILE_VERSION_STRING
                           " (C) Albin Johansson 2020-2022");

    ImGui::Separator();
    ImGui::TextUnformatted(strings.misc.license_info.c_str());

    ImGui::Spacing();
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(strings.misc.repository_link.c_str());

    ImGui::SameLine();
    if (push_button(TAC_ICON_LINK, strings.tooltip.repository_link.c_str())) {
      cen::open_url("https://www.github.com/albin-johansson/tactile");
    }

    ImGui::Spacing();
    ImGui::TextUnformatted(strings.misc.font_awesome_credit.c_str());
  }
}

}  // namespace tactile::ui
