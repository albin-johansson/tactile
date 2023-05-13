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

#include "add_component_attr_dialog.hpp"

#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "common/util/vector.hpp"
#include "core/component.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct AddComponentAttrDialogState final {
  Maybe<ComponentDefinitionEntity> definition_entity;
  StringBuffer attribute_name_buffer {};
  bool open_dialog {};
};

inline AddComponentAttrDialogState gDialogState;

}  // namespace

void open_create_component_attribute_dialog(const Entity component_definition_entity)
{
  gDialogState.definition_entity = component_definition_entity;
  gDialogState.attribute_name_buffer.clear();
  gDialogState.open_dialog = true;
}

void update_create_component_attribute_dialog(const Model& model, Dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);
  const auto& component_set = model.get<ComponentSet>(document.component_set);

  if (gDialogState.definition_entity.has_value() &&
      !contains_value(component_set.definitions, *gDialogState.definition_entity)) {
    gDialogState.definition_entity.reset();
    gDialogState.open_dialog = false;
    return;
  }

  DialogOptions options {
      .title = lang.window.create_attribute.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.create.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  const auto& component_def =
      model.get<ComponentDefinition>(gDialogState.definition_entity.value());
  const auto current_name = gDialogState.attribute_name_buffer.as_string_view();

  if (!current_name.empty() && !component_def.attributes.contains(current_name)) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::InputTextWithHint("##Name",
                             lang.misc.attribute_name_hint.c_str(),
                             gDialogState.attribute_name_buffer.data(),
                             sizeof gDialogState.attribute_name_buffer);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<AddComponentAttrEvent>(
        gDialogState.definition_entity.value(),
        gDialogState.attribute_name_buffer.as_string());
  }
}

}  // namespace tactile::ui