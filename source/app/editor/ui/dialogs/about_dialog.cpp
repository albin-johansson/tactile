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

#include "editor/lang/language.hpp"
#include "editor/lang/strings.hpp"
#include "editor/ui/common/buttons.hpp"
#include "editor/ui/icons.hpp"
#include "meta/build.hpp"

namespace tactile::ui {

AboutDialog::AboutDialog()
    : ADialog {"About Tactile"}
{
  set_accept_button_label(nothing);
}

void AboutDialog::show()
{
  const auto& lang = get_current_language();
  set_title(lang.window.about_tactile);
  set_close_button_label(lang.misc.close);
  make_visible();
}

void AboutDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  ImGui::TextUnformatted("Tactile " TACTILE_VERSION_STRING
                         " (C) Albin Johansson 2020-2022");
  ImGui::Separator();

  ImGui::TextUnformatted(lang.misc.license_info.c_str());

  ImGui::Spacing();

  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted(lang.misc.repository_link.c_str());

  ImGui::SameLine();
  if (button(TAC_ICON_LINK, lang.tooltip.repository_link.c_str())) {
    cen::open_url("https://www.github.com/albin-johansson/tactile");
  }

  ImGui::Spacing();

  ImGui::TextUnformatted(lang.misc.font_awesome_credit.c_str());
}

}  // namespace tactile::ui
