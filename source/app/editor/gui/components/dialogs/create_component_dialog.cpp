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

#include "create_component_dialog.hpp"

#include <string>  // string

#include "editor/events/component_events.hpp"

namespace tactile {

create_component_dialog::create_component_dialog()
    : component_name_dialog{"Create Component"}
{
  set_accept_button_label("Create");
  set_input_hint("Component name");
}

void create_component_dialog::show()
{
  component_name_dialog::show("");
}

void create_component_dialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CreateComponentDefEvent>(std::string{current_input()});
}

}  // namespace tactile