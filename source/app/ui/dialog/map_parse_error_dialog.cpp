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

#include "map_parse_error_dialog.hpp"

#include <fmt/format.h>
#include <imgui.h>

#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {

MapParseErrorDialog::MapParseErrorDialog()
    : Dialog {"Map Parse Error"}
{
  set_close_button_label(nothing);
}

void MapParseErrorDialog::show(const io::ParseError error)
{
  const auto& lang = get_current_language();

  mError = error;
  mCause = fmt::format("{}: {}", lang.misc.cause, io::to_cause(mError.value()));

  set_title(lang.window.map_parse_error);

  make_visible();
}

void MapParseErrorDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  const auto& lang = get_current_language();

  ImGui::TextUnformatted(lang.misc.map_parse_error.c_str());
  ImGui::TextUnformatted(mCause.c_str());
}

}  // namespace tactile::ui
