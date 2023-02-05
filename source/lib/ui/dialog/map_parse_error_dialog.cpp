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

#include "map_parse_error_dialog.hpp"

#include <fmt/format.h>
#include <imgui.h>

#include "common/type/string.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct MapParseErrorDialogState final {
  String cause;
  bool open_dialog {};
};

inline MapParseErrorDialogState gDialogState;

}  // namespace

void open_map_parse_error_dialog(const ParseError error)
{
  const auto& lang = get_current_language();
  gDialogState.cause = fmt::format("{}: {}", lang.misc.cause, to_cause(error));
  gDialogState.open_dialog = true;
}

void update_map_parse_error_dialog()
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.map_parse_error.c_str(),
      .close_label = lang.misc.close.c_str(),
      .flags = UI_DIALOG_FLAG_INPUT_IS_VALID,
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (const ScopedDialog dialog {options}; dialog.was_opened()) {
    ImGui::TextUnformatted(lang.misc.map_parse_error.c_str());
    ImGui::TextUnformatted(gDialogState.cause.c_str());
  }
}

}  // namespace tactile::ui
