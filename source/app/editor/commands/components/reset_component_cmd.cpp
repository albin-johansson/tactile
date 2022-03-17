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

#include "reset_component_cmd.hpp"

namespace tactile {

ResetComponentCmd::ResetComponentCmd(RegistryRef registry,
                                     const context_id contextId,
                                     const component_id componentId)
    : ACommand{"Reset Component Values"}
    , mRegistry{registry}
    , mContextId{contextId}
    , mComponentId{componentId}
{}

void ResetComponentCmd::undo()
{
  auto& registry = mRegistry.get();
  const auto& snapshot = mSnapshot.value();

  for (const auto& [name, value] : snapshot.values) {
    sys::update_component(registry, mContextId, mComponentId, name, value);
  }

  mSnapshot.reset();
}

void ResetComponentCmd::redo()
{
  auto& registry = mRegistry.get();
  mSnapshot = sys::reset_component(registry, mContextId, mComponentId);
}

}  // namespace tactile