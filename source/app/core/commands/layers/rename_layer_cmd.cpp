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

#include "rename_layer_cmd.hpp"

#include <utility>  // move

#include "core/common/ecs.hpp"
#include "core/components/attributes.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

RenameLayerCmd::RenameLayerCmd(RegistryRef registry, const LayerID id, std::string name)
    : ACommand{"Rename Layer"}
    , mRegistry{registry}
    , mLayerId{id}
    , mName{std::move(name)}
{}

void RenameLayerCmd::undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = checked_get<comp::AttributeContext>(registry, entity);
  context.name = mPreviousName.value();

  mPreviousName.reset();
}

void RenameLayerCmd::redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::find_layer(registry, mLayerId);
  TACTILE_ASSERT(entity != entt::null);

  auto& context = checked_get<comp::AttributeContext>(registry, entity);
  mPreviousName = context.name;
  context.name = mName;
}

}  // namespace tactile
