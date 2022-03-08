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

#include "remove_component_cmd.hpp"

namespace tactile {

remove_component_cmd::remove_component_cmd(registry_ref registry,
                                           const context_id contextId,
                                           const component_id componentId)
    : command_base{"Remove Component"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void remove_component_cmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_component(registry, mSnapshot.value());
  mSnapshot.reset();
}

void remove_component_cmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::remove_component(registry, mContextId, mComponentId);
}

}  // namespace tactile