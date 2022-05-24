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

#include "add_property_dialog.hpp"

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "core/systems/context_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/utils/buffers.hpp"
#include "editor/events/property_events.hpp"
#include "editor/gui/properties/dialogs/property_type_combo.hpp"
#include "editor/model.hpp"

namespace tactile {

AddPropertyDialog::AddPropertyDialog() : ADialog{"Add Property"}
{
  set_accept_button_label("Add");
}

void AddPropertyDialog::open()
{
  zero_buffer(mNameBuffer);
  mPropertyType = AttributeType::String;
  make_visible();
}

void AddPropertyDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  ImGui::InputTextWithHint("##Name",
                           "Unique property name...",
                           mNameBuffer.data(),
                           sizeof mNameBuffer);
  show_property_type_combo(mPropertyType);
}

void AddPropertyDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<AddPropertyEvent>(create_string_from_buffer(mNameBuffer),
                                       mPropertyType);
}

auto AddPropertyDialog::is_current_input_valid(const DocumentModel& model) const -> bool
{
  const auto& registry = model.get_active_registry();
  const auto& context = sys::current_context(registry);

  const auto name = create_string_view_from_buffer(mNameBuffer);
  return !name.empty() && !sys::has_property_with_name(registry, context, name);
}

}  // namespace tactile
