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

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "core/attribute.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/property_events.hpp"
#include "model/model.hpp"
#include "tactile/core/type/maybe.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/attribute_widgets.hpp"

namespace tactile::ui {
namespace {

struct AddPropertyDialogState final {
  Maybe<UUID> context_id;
  StringBuffer name_buffer {};
  AttributeType property_type {AttributeType::String};
  bool open_dialog {};
};

inline AddPropertyDialogState gDialogState;

}  // namespace

void open_add_property_dialog(const UUID& context_id)
{
  gDialogState.context_id = context_id;
  gDialogState.name_buffer.clear();
  gDialogState.property_type = AttributeType::String;
  gDialogState.open_dialog = true;
}

void update_add_property_dialog(const DocumentModel& model, entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  const auto& document = model.require_active_document();
  const auto& active_context = document.get_contexts().get_active_context();

  if (active_context.get_uuid() != gDialogState.context_id) {
    gDialogState.context_id.reset();
    gDialogState.open_dialog = false;
    return;
  }

  DialogOptions options {
      .title = lang.window.add_property.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.add.c_str(),
  };

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  const auto current_name = gDialogState.name_buffer.as_string_view();
  if (!current_name.empty() && !active_context.get_ctx().has_property(current_name)) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    ImGui::InputTextWithHint("##Name",
                             lang.misc.property_name_hint.c_str(),
                             gDialogState.name_buffer.data(),
                             sizeof gDialogState.name_buffer);
    if (const auto new_type = ui_attribute_type_combo(gDialogState.property_type)) {
      gDialogState.property_type = *new_type;
    }
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<AddPropertyEvent>(gDialogState.context_id.value(),
                                         gDialogState.name_buffer.as_string(),
                                         gDialogState.property_type);
    gDialogState.context_id.reset();
  }
}

}  // namespace tactile::ui
