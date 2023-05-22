/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "define_component.hpp"

#include <utility>  // move

#include "model/context.hpp"
#include "model/systems/component/component_set.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

DefineComponent::DefineComponent(const Entity component_set_entity, String name)
    : mComponentSetEntity {component_set_entity},
      mName {std::move(name)}
{
}

void DefineComponent::undo()
{
  auto& model = get_global_model();
  auto& component_set = model.get<ComponentSet>(mComponentSetEntity);
  sys::remove_component(model, component_set, mName);
}

void DefineComponent::redo()
{
  auto& model = get_global_model();
  auto& component_set = model.get<ComponentSet>(mComponentSetEntity);
  sys::create_component(model, component_set, mName);
}

auto DefineComponent::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.define_comp;
}

}  // namespace tactile::cmd
