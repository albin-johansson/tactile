// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

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
#include "tactile/base/container/maybe.hpp"
#include "ui/dialog/dialog.hpp"
#include "ui/widget/attribute_widgets.hpp"

namespace tactile::ui {
namespace {

struct AddPropertyDialogState final {
  Maybe<UUID> context_id;
  StringBuffer name_buffer {};
  AttributeType property_type {AttributeType::kStr};
  bool open_dialog {};
};

inline AddPropertyDialogState gDialogState;

}  // namespace

void open_add_property_dialog(const UUID& context_id)
{
  gDialogState.context_id = context_id;
  gDialogState.name_buffer.clear();
  gDialogState.property_type = AttributeType::kStr;
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
