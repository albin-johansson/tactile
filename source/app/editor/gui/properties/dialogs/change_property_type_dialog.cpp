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

#include "change_property_type_dialog.hpp"

#include <utility>  // move

#include <entt/signal/dispatcher.hpp>
#include <imgui.h>

#include "editor/events/property_events.hpp"
#include "editor/gui/common/button.hpp"
#include "property_type_combo.hpp"

namespace tactile {

ChangePropertyTypeDialog::ChangePropertyTypeDialog() : ADialog{"Change Property Type"}
{
  set_accept_button_label("Change");
}

void ChangePropertyTypeDialog::show(std::string name, const AttributeType type)
{
  mPropertyName = std::move(name);
  mCurrentType = type;
  mPreviousType = type;
  make_visible();
}

void ChangePropertyTypeDialog::on_update(const DocumentModel&, entt::dispatcher&)
{
  ImGui::AlignTextToFramePadding();
  ImGui::TextUnformatted("Type: ");

  ImGui::SameLine();
  show_property_type_combo(mPreviousType.value(), mCurrentType);
}

void ChangePropertyTypeDialog::on_accept(entt::dispatcher& dispatcher)
{
  dispatcher.enqueue<ChangePropertyTypeEvent>(mPropertyName.value(), mCurrentType);
}

auto ChangePropertyTypeDialog::is_current_input_valid(const DocumentModel&) const -> bool
{
  return mCurrentType != mPreviousType.value();
}

}  // namespace tactile
