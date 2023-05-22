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

#include "add_property_dialog.hpp"

#include <imgui.h>

#include "common/attribute.hpp"
#include "common/type/maybe.hpp"
#include "common/util/string_buffer.hpp"
#include "components/document.hpp"
#include "core/context.hpp"
#include "model/event/property_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/attribute_widgets.hpp"

namespace tactile::ui {
namespace {

struct AddPropertyDialogState final {
  Maybe<Entity> context_entity;
  StringBuffer name_buffer {};
  AttributeType property_type {AttributeType::String};
  bool open_dialog {};
};

inline AddPropertyDialogState gDialogState;

}  // namespace

void open_add_property_dialog(const Entity context_entity)
{
  gDialogState.context_entity = context_entity;
  gDialogState.name_buffer.clear();
  gDialogState.property_type = AttributeType::String;
  gDialogState.open_dialog = true;
}

void update_add_property_dialog(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);

  if (document.active_context != gDialogState.context_entity) {
    gDialogState.context_entity.reset();
    gDialogState.open_dialog = false;
    return;
  }

  DialogOptions options {
      .title = strings.window.add_property.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.add.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  const auto& active_context = model.get<Context>(document.active_context);

  const auto current_name = gDialogState.name_buffer.as_string_view();
  if (!current_name.empty() && !active_context.props.contains(current_name)) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::InputTextWithHint("##Name",
                             strings.misc.property_name_hint.c_str(),
                             gDialogState.name_buffer.data(),
                             sizeof gDialogState.name_buffer);
    if (const auto new_type =
            push_attribute_type_combo(strings, gDialogState.property_type)) {
      gDialogState.property_type = *new_type;
    }
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<AddPropertyEvent>(gDialogState.context_entity.value(),
                                         gDialogState.name_buffer.as_string(),
                                         gDialogState.property_type);
    gDialogState.context_entity.reset();
  }
}

}  // namespace tactile::ui
