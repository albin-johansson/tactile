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

#include "common/type/string.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

inline String dialog_cause;
inline constinit bool show_dialog = false;

}  // namespace

void open_map_parse_error_dialog(const io::ParseError error)
{
  const auto& lang = get_current_language();
  dialog_cause = fmt::format("{}: {}", lang.misc.cause, io::to_cause(error));
  show_dialog = true;
}

void update_map_parse_error_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.map_parse_error.c_str(),
      .close_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (show_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    show_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.map_parse_error.c_str());
    ImGui::TextUnformatted(dialog_cause.c_str());
  }
}

}  // namespace tactile::ui