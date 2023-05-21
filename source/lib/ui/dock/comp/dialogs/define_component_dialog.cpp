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

#include "define_component_dialog.hpp"

#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "model/document.hpp"
#include "model/event/component_events.hpp"
#include "model/systems/component/component_set.hpp"
#include "model/systems/document_system.hpp"
#include "systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct DefineComponentDialogState final {
  StringBuffer component_name_buffer {};
  bool open_dialog {};
};

inline DefineComponentDialogState gDialogState;

}  // namespace

void open_define_component_dialog()
{
  gDialogState.component_name_buffer.clear();
  gDialogState.open_dialog = true;
}

void update_define_component_dialog(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  DialogOptions options {
      .title = strings.window.create_component.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.create.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  const auto current_component_name = gDialogState.component_name_buffer.as_string_view();

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);

  if (!current_component_name.empty() &&
      sys::find_component_definition(model,
                                     document.component_set,
                                     current_component_name) != kNullEntity) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::InputTextWithHint("##Name",
                             strings.misc.component_name_hint.c_str(),
                             gDialogState.component_name_buffer.data(),
                             sizeof gDialogState.component_name_buffer);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<DefineComponentEvent>(
        gDialogState.component_name_buffer.as_string());
  }
}

}  // namespace tactile::ui