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

#include "set_property_type_dialog.hpp"

#include <imgui.h>

#include "model/components/document.hpp"
#include "model/documents/document_system.hpp"
#include "model/event/property_events.hpp"
#include "model/model.hpp"
#include "model/systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/attribute_widgets.hpp"

namespace tactile::ui {

void push_set_property_type_dialog(const Model& model,
                                   SetPropertyTypeDialogState& state,
                                   Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);

  if (document.active_context != state.context) {
    state.context = kNullEntity;
    state.should_open = false;
    return;
  }

  DialogOptions dialog_options {
      .title = strings.window.change_property_type.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.change.c_str(),
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  if (state.current_type != state.old_type.value()) {
    dialog_options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {dialog_options, &action}; dialog.was_opened()) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(strings.misc.type.c_str());

    ImGui::SameLine();
    if (const auto new_type = push_attribute_type_combo(strings,
                                                        state.current_type,
                                                        state.old_type.value())) {
      state.current_type = *new_type;
    }
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<SetPropertyTypeEvent>(state.context,
                                             state.property_name.value(),
                                             state.current_type);
    state.context = kNullEntity;
  }
}

}  // namespace tactile::ui
