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

#include "new_comp_dialog.hpp"

#include <imgui.h>

#include "model/components/component_set_system.hpp"
#include "model/documents/document_components.hpp"
#include "model/documents/document_system.hpp"
#include "model/events/component_events.hpp"
#include "model/i18n/language_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile {

void push_new_comp_dialog(ModelView model, NewCompDialogState& state)
{
  const auto& registry = model.get_registry();
  const auto& strings = model.get_language_strings();

  ui::DialogOptions dialog_options {
      .title = strings.window.create_component.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.create.c_str(),
  };

  if (state.should_open) {
    dialog_options.flags |= ui::UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  const auto document_entity = sys::get_active_document(registry);
  const auto& document = registry.get<Document>(document_entity);
  const auto& component_set = registry.get<ComponentSet>(document.component_set);

  const auto current_name = state.name_buffer.as_string_view();
  if (!current_name.empty() &&
      sys::find_component(registry, component_set, current_name) == kNullEntity) {
    dialog_options.flags |= ui::UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  ui::DialogAction action {ui::DialogAction::None};
  if (const ui::ScopedDialog dialog {dialog_options, &action}; dialog.was_opened()) {
    ImGui::InputTextWithHint("##Name",
                             strings.misc.component_name_hint.c_str(),
                             state.name_buffer.data(),
                             state.name_buffer.size_bytes());
  }

  if (action == ui::DialogAction::Accept) {
    model.enqueue<DefineComponentEvent>(state.name_buffer.as_string());
  }
}

}  // namespace tactile