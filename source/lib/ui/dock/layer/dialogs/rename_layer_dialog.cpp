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

#include "rename_layer_dialog.hpp"

#include <imgui.h>

#include "model/events/layer_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {

void push_rename_layer_dialog(const Model& model,
                              RenameLayerDialogState& state,
                              Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions dialog_options {
      .title = strings.window.rename_layer.c_str(),
      .close_label = strings.misc.close.c_str(),
      .accept_label = strings.misc.rename.c_str(),
  };

  const bool should_acquire_focus = state.should_open;

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (auto new_name = state.name_buffer.as_string_view();
      !new_name.empty() && new_name != state.old_name) {
    dialog_options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {dialog_options, &action}; dialog.was_opened()) {
    if (should_acquire_focus) {
      ImGui::SetKeyboardFocusHere();
    }
    ImGui::InputText("##Input", state.name_buffer.data(), state.name_buffer.size_bytes());
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenameLayerEvent>(state.layer, state.name_buffer.as_string());
  }
}

}  // namespace tactile::ui
