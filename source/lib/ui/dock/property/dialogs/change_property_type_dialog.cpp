// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

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

struct ChangePropertyTypeDialogState final {
  Maybe<UUID> context_id;
  AttributeType current_type {AttributeType::kStr};
  Maybe<String> property_name;
  Maybe<AttributeType> previous_type;
  bool open_dialog {};
};

inline ChangePropertyTypeDialogState gDialogState;

}  // namespace

void open_change_property_type_dialog(const UUID& context_id,
                                      String property_name,
                                      const AttributeType property_type)
{
  gDialogState.context_id = context_id;
  gDialogState.property_name = std::move(property_name);
  gDialogState.previous_type = property_type;
  gDialogState.current_type = property_type;
  gDialogState.open_dialog = true;
}

void update_change_property_type_dialog(const DocumentModel& model,
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
      .title = lang.window.change_property_type.c_str(),
      .close_label = lang.misc.cancel.c_str(),
      .accept_label = lang.misc.change.c_str(),
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
    ImGui::TextUnformatted(lang.misc.type.c_str());

    ImGui::SameLine();
    if (const auto new_type =
            ui_attribute_type_combo(gDialogState.current_type,
                                    gDialogState.previous_type.value())) {
      gDialogState.current_type = *new_type;
    }
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<ChangePropertyTypeEvent>(gDialogState.context_id.value(),
                                                gDialogState.property_name.value(),
                                                gDialogState.current_type);
    gDialogState.context_id.reset();
  }
}

}  // namespace tactile::ui
