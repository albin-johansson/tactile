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

#include "rename_layer.hpp"

#include <utility>  // move

#include "common/debug/assert.hpp"
#include "model/contexts/context_components.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"

namespace tactile::cmd {

RenameLayer::RenameLayer(Registry* registry, const Entity layer_entity, String name)
    : mRegistry {registry},
      mLayerEntity {layer_entity},
      mNewName {std::move(name)}
{
  TACTILE_ASSERT(sys::is_layer_entity(*mRegistry, mLayerEntity));
}

void RenameLayer::undo()
{
  auto& registry = *mRegistry;

  auto& layer_context = registry.get<Context>(mLayerEntity);
  layer_context.name = mOldName.value();

  mOldName.reset();
}

void RenameLayer::redo()
{
  auto& registry = *mRegistry;
  auto& layer_context = registry.get<Context>(mLayerEntity);

  mOldName = layer_context.name;
  layer_context.name = mNewName;
}

auto RenameLayer::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.rename_layer;
}

}  // namespace tactile::cmd
