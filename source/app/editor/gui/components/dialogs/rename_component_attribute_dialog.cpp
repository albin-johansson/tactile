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

#include "rename_component_attribute_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>

#include "core/events/component_events.hpp"
#include "core/model.hpp"
#include "core/systems/component_system.hpp"

namespace tactile {

RenameComponentAttributeDialog::RenameComponentAttributeDialog()
    : AStringInputDialog{"Rename Component Attribute"}
{
  set_accept_button_label("Rename");
  set_input_hint("Attribute name");
}

void RenameComponentAttributeDialog::show(std::string previousName, const ComponentID& id)
{
  mComponentId = id;
  AStringInputDialog::show(std::move(previousName));
}

void RenameComponentAttributeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenameComponentAttrEvent>(mComponentId.value(),
                                               previous_input(),
                                               std::string{current_input()});
}

auto RenameComponentAttributeDialog::validate(const DocumentModel& model,
                                              const std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  return !input.empty() &&
         !sys::is_component_attribute_name_taken(registry, mComponentId.value(), input);
}

}  // namespace tactile