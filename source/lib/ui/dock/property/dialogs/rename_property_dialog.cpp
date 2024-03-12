// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_property_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "core/context/context.hpp"
#include "core/context/context_info.hpp"
#include "core/context/context_manager.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/property_events.hpp"
#include "model/model.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct RenamePropertyDialogState final {
  Maybe<UUID> context_id;
  String previous_name;
  StringBuffer name_buffer {};
  bool open_dialog {};
};

inline RenamePropertyDialogState gDialogState;

}  // namespace

void open_rename_property_dialog(const UUID& context_id, String previous_name)
{
  gDialogState.context_id = context_id;
  gDialogState.previous_name = std::move(previous_name);
  gDialogState.name_buffer.clear();
  gDialogState.open_dialog = true;
}

void update_rename_property_dialog(const DocumentModel& model,
                                   entt::dispatcher& dispatcher)
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
      .title = lang.window.rename_property.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.rename.c_str(),
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
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenamePropertyEvent>(gDialogState.context_id.value(),
                                            gDialogState.previous_name,
                                            gDialogState.name_buffer.as_string());
    gDialogState.context_id.reset();
  }
}

}  // namespace tactile::ui
