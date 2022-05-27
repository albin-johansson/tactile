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

#include "create_component_attribute_dialog.hpp"

#include <string>  // string

#include <entt/signal/dispatcher.hpp>

#include "core/systems/component_system.hpp"
#include "core/events/component_events.hpp"
#include "editor/model.hpp"

namespace tactile {

CreateComponentAttributeDialog::CreateComponentAttributeDialog()
    : AStringInputDialog{"Create Attribute"}
{
  set_accept_button_label("Create");
  set_input_hint("Attribute name");
}

void CreateComponentAttributeDialog::show(const ComponentID& id)
{
  mComponentId = id;
  AStringInputDialog::show("");
}

auto CreateComponentAttributeDialog::validate(const DocumentModel& model,
                                              const std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() &&
         !sys::is_component_attribute_name_taken(registry, mComponentId, input);
}

void CreateComponentAttributeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<CreateComponentAttrEvent>(mComponentId,
                                               std::string{current_input()});
}

}  // namespace tactile