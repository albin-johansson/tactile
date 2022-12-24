/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
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

#include "rename_layer_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/type/maybe.hpp"
#include "core/util/string_buffer.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"
#include "model/event/layer_events.hpp"
#include "ui/dialog/dialog.hpp"

namespace tactile::ui {
namespace {

inline Maybe<UUID> dialog_layer_id;
inline String dialog_old_name;
inline StringBuffer dialog_name_buffer;
inline constinit bool open_dialog = false;

}  // namespace

void open_rename_layer_dialog(const UUID& layer_id, String current_name)
{
  dialog_layer_id = layer_id;
  dialog_old_name = std::move(current_name);
  dialog_name_buffer = dialog_old_name;
  open_dialog = true;
}

void update_rename_layer_dialog(entt::dispatcher& dispatcher)
{
  const auto& lang = get_current_language();

  DialogOptions options {
      .title = lang.window.rename_layer.c_str(),
      .close_label = lang.misc.close.c_str(),
      .accept_label = lang.misc.rename.c_str(),
  };

  const bool should_acquire_focus = open_dialog;

  if (open_dialog) {
    options.flags |= UI_DIALOG_FLAG_OPEN;
    open_dialog = false;
  }

  if (auto new_name = dialog_name_buffer.as_string_view();
      !new_name.empty() && new_name != dialog_old_name) {
    options.flags |= UI_DIALOG_FLAG_INPUT_IS_VALID;
  }

  DialogAction action {DialogAction::None};
  if (const ScopedDialog dialog {options, &action}; dialog.was_opened()) {
    if (should_acquire_focus) {
      ImGui::SetKeyboardFocusHere();
    }
    ImGui::InputTextWithHint("##Input",
                             "",
                             dialog_name_buffer.data(),
                             sizeof dialog_name_buffer);
  }

  if (action == DialogAction::Accept) {
    dispatcher.enqueue<RenameLayerEvent>(dialog_layer_id.value(),
                                         dialog_name_buffer.as_string());
  }
}

}  // namespace tactile::ui
