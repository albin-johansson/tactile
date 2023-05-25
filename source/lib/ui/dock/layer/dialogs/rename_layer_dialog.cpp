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

#include <utility>  // move

#include <imgui.h>

#include "common/type/maybe.hpp"
#include "common/util/string_buffer.hpp"
#include "model/event/layer_events.hpp"
#include "model/systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct RenameLayerDialogState final {
  Maybe<Entity> layer_entity;
  String old_name;
  StringBuffer name_buffer {};
  bool open_dialog {};
};

inline RenameLayerDialogState gDialogState;

}  // namespace

void open_rename_layer_dialog(const Entity layer_entity, String current_name)
{
  gDialogState.layer_entity = layer_entity;
  gDialogState.old_name = std::move(current_name);
  gDialogState.name_buffer = gDialogState.old_name;
  gDialogState.open_dialog = true;
}

void update_rename_layer_dialog(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions options {
      .title = strings.window.rename_layer.c_str(),
      .close_label = strings.misc.close.c_str(),
      .accept_label = strings.misc.rename.c_str(),
  };

  const bool should_acquire_focus = gDialogState.open_dialog;

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (auto new_name = gDialogState.name_buffer.as_string_view();
      !new_name.empty() && new_name != gDialogState.old_name) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    if (should_acquire_focus) {
      ImGui::SetKeyboardFocusHere();
    }
    ImGui::InputText("##Input",
                     gDialogState.name_buffer.data(),
                     sizeof gDialogState.name_buffer);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenameLayerEvent>(gDialogState.layer_entity.value(),
                                         gDialogState.name_buffer.as_string());
  }
}

}  // namespace tactile::ui
