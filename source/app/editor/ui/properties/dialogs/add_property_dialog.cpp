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

#include "core/context/context.hpp"
#include "core/event/property_events.hpp"
#include "core/model.hpp"
#include "core/property_bundle.hpp"
#include "core/util/buffers.hpp"
#include "editor/ui/properties/dialogs/property_type_combo.hpp"

namespace tactile::ui {

AddPropertyDialog::AddPropertyDialog() : ADialog {"Add Property"}
{
  set_accept_button_label("Add");
}

void AddPropertyDialog::open(const UUID& contextId)
{
  mContextId = contextId;
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
  dispatcher.enqueue<AddPropertyEvent>(mContextId.value(),
                                       create_string_from_buffer(mNameBuffer),
                                       mPropertyType);
  mContextId.reset();
}

auto AddPropertyDialog::is_current_input_valid(const DocumentModel& model) const -> bool
{
  const auto& document = model.require_active_document();
  const auto& context = document.view_context(document.active_context_id());
  const auto  name = create_string_view_from_buffer(mNameBuffer);
  return !name.empty() && !context.get_props().contains(name);
}

}  // namespace tactile::ui
