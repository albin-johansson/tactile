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

#include "rename_property_dialog.hpp"

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "editor/events/property_events.hpp"
#include "editor/model.hpp"

namespace tactile {

RenamePropertyDialog::RenamePropertyDialog() : string_input_dialog{"Rename Property"}
{
  set_accept_button_label("Rename");
}

void RenamePropertyDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<RenamePropertyEvent>(previous_input(), std::string{current_input()});
}

auto RenamePropertyDialog::validate(const document_model& model,
                                    std::string_view input) const -> bool
{
  const auto& registry = model.get_active_registry();
  const auto& context = sys::current_context(registry);
  return !input.empty() && !sys::has_property_with_name(registry, context, input);
}

}  // namespace tactile
