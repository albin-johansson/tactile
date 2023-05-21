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

#include "rename_component_dialog.hpp"

#include <utility>  // move

#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "core/component.hpp"
#include "model/document.hpp"
#include "model/event/component_events.hpp"
#include "model/model.hpp"
#include "model/systems/component/component_set.hpp"
#include "model/systems/document_system.hpp"
#include "systems/language_system.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct RenameComponentDialogState final {
  Maybe<Entity> definition_entity;
  String old_component_name;
  StringBuffer component_name_buffer;
  bool open_dialog {};
};

inline RenameComponentDialogState gDialogState;

}  // namespace

void open_rename_component_dialog(const Entity definition_entity, String current_name)
{
  gDialogState.definition_entity = definition_entity;
  gDialogState.component_name_buffer = current_name;
  gDialogState.old_component_name = std::move(current_name);
  gDialogState.open_dialog = true;
}

void update_rename_component_dialog(const Model& model, Dispatcher& dispatcher)
{
  const auto& strings = sys::get_current_language_strings(model);

  const auto document_entity = sys::get_active_document(model);
  const auto& document = model.get<Document>(document_entity);
  const auto& component_set = model.get<ComponentSet>(document.component_set);

  if (gDialogState.definition_entity.has_value() &&
      !component_set.has_component(*gDialogState.definition_entity)) {
    gDialogState.definition_entity.reset();
    gDialogState.open_dialog = false;
    return;
  }

  DialogOptions options {
      .title = strings.window.rename_component.c_str(),
      .close_label = strings.misc.cancel.c_str(),
      .accept_label = strings.misc.rename.c_str(),
  };

  const bool should_acquire_focus = gDialogState.open_dialog;

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  const auto current_name = gDialogState.component_name_buffer.as_string_view();
  if (!current_name.empty() &&
      sys::find_component_definition(model, component_set, current_name) == kNullEntity) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    if (should_acquire_focus) {
      ImGui::SetKeyboardFocusHere();
    }
    ImGui::InputText("##Input",
                     gDialogState.component_name_buffer.data(),
                     sizeof gDialogState.component_name_buffer);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenameComponentEvent>(
        gDialogState.definition_entity.value(),
        gDialogState.component_name_buffer.as_string());
  }
}

}  // namespace tactile::ui