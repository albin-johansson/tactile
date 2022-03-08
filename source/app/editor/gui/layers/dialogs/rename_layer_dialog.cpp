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

#include "editor/events/layer_events.hpp"

namespace tactile {

rename_layer_dialog::rename_layer_dialog() : string_input_dialog{"Rename Layer"}
{
  set_accept_button_label("Rename");
}

void rename_layer_dialog::show(const layer_id id, std::string oldName)
{
  mTargetId = id;
  mOldName = std::move(oldName);
  string_input_dialog::show(*mOldName);
}

void rename_layer_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<rename_layer_event>(mTargetId.value(), std::string{current_input()});
}

auto rename_layer_dialog::validate(const document_model&, std::string_view input) const
    -> bool
{
  return !input.empty() && mOldName != input;
}

}  // namespace tactile
