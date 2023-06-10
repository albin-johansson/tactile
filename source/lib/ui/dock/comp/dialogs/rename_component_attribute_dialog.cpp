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

#include "rename_component_attribute_dialog.hpp"

#include <imgui.h>

#include "common/util/vectors.hpp"
#include "components/component.hpp"
#include "components/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {

void push_rename_comp_attr_dialog(const Model& model,
                                  RenameCompAttrDialogState& state,
                                  Dispatcher& dispatcher)
{
  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);

  const auto& component_set = model.get<ComponentSet>(document.component_set);
  if (!contained_in(component_set.definitions, state.definition)) {
    state.definition = kNullEntity;
    state.should_open = false;
    return;
  }

  const auto& strings = sys::get_current_language_strings(model);
  DialogOptions dialog_options {
      .title = strings.window.rename_component_attribute.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.rename.c_str(),
  };

  if (state.should_open) {
    dialog_options.flags |= UI_DIALOG_FLAG_OPEN;
    state.should_open = false;
  }

  const auto current_name = state.attribute_name_buffer.as_string_view();
  const auto& definition = model.get<ComponentDefinition>(state.definition);

  if (!current_name.empty() && !definition.attributes.contains(current_name)) {
    dialog_options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {dialog_options, &action}; dialog.was_opened()) {
    ImGui::InputTextWithHint("##Name",
                             strings.misc.attribute_name_hint.c_str(),
                             state.attribute_name_buffer.data(),
                             state.attribute_name_buffer.size_bytes());
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenameComponentAttrEvent>(state.definition,
                                                 state.attribute_name,
                                                 state.attribute_name_buffer.as_string());
  }
}

}  // namespace tactile::ui