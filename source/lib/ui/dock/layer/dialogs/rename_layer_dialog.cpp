// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "rename_layer_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "common/util/string_buffer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/layer_events.hpp"
#include "tactile/base/container/maybe.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

struct RenameLayerDialogState final {
  Maybe<UUID> layer_id;
  String old_name;
  StringBuffer name_buffer {};
  bool open_dialog {};
};

inline RenameLayerDialogState gDialogState;

}  // namespace

void open_rename_layer_dialog(const UUID& layer_id, String current_name)
{
  gDialogState.layer_id = layer_id;
  gDialogState.old_name = std::move(current_name);
  gDialogState.name_buffer = gDialogState.old_name;
  gDialogState.open_dialog = true;
}

void update_rename_layer_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.rename_layer.c_str(),
      .close_label = lang.misc.close.c_str(),
      .accept_label = lang.misc.rename.c_str(),
  };

  const bool should_acquire_focus = gDialogState.open_dialog;

  if (gDialogState.open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    gDialogState.open_dialog = false;
  }

  if (auto new_name = gDialogState.name_buffer.as_string_view();
      !new_name.empty() && new_name != gDialogState.old_name) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    if (should_acquire_focus) {
      ImGui::SetKeyboardFocusHere();
    }
    ImGui::InputText("##Input",
                     gDialogState.name_buffer.data(),
                     sizeof gDialogState.name_buffer);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenameLayerEvent>(gDialogState.layer_id.value(),
                                         gDialogState.name_buffer.as_string());
  }
}

}  // namespace tactile::ui
