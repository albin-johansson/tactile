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

#include "rename_component_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "core/event/component_events.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::ui {

RenameComponentDialog::RenameComponentDialog()
    : ComponentNameDialog {"Rename Component"}
{
}

void RenameComponentDialog::show(std::string previous_name, const UUID& component_id)
{
  mComponentId = component_id;

  const auto& lang = get_current_language();
  set_title(lang.window.rename_component);
  set_accept_button_label(lang.misc.rename);

  ComponentNameDialog::show(std::move(previous_name));
}

void RenameComponentDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenameComponentEvent>(mComponentId.value(),
                                           std::string {current_input()});
}

}  // namespace tactile::ui