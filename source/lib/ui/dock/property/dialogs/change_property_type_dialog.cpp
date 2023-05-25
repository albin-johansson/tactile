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

#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "common/type/maybe.hpp"
#include "components/document.hpp"
#include "model/event/property_events.hpp"
#include "model/model.hpp"
#include "model/systems/document_system.hpp"
#include "model/systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/attribute_widgets.hpp"

namespace tactile::ui {
namespace {

struct ChangePropertyTypeDialogState final {
  Maybe<Entity> context_entity;
  AttributeType current_type {AttributeType::String};
  Maybe<String> property_name;
  Maybe<AttributeType> previous_type;
  bool open_dialog {};
};

inline ChangePropertyTypeDialogState gDialogState;

}  // namespace

void open_change_property_type_dialog(const Entity context_entity,
                                      String property_name,
                                      const AttributeType property_type)
{
  gDialogState.context_entity = context_entity;
  gDialogState.property_name = std::move(property_name);
  gDialogState.previous_type = property_type;
  gDialogState.current_type = property_type;
  gDialogState.open_dialog = true;
}

void update_change_property_type_dialog(const Model& model, Dispatcher& dispatcher)
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
      .title = strings.window.change_property_type.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.change.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (gDialogState.current_type != gDialogState.previous_type.value()) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::AlignTextToFramePadding();
    ImGui::TextUnformatted(strings.misc.type.c_str());

    ImGui::SameLine();
    if (const auto new_type =
            push_attribute_type_combo(strings,
                                      gDialogState.current_type,
                                      gDialogState.previous_type.value())) {
      gDialogState.current_type = *new_type;
    }
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<ChangePropertyTypeEvent>(gDialogState.context_entity.value(),
                                                gDialogState.property_name.value(),
                                                gDialogState.current_type);
    gDialogState.context_entity.reset();
  }
}

}  // namespace tactile::ui
