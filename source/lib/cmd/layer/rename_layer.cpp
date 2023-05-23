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

#include "components/context.hpp"
#include "model/context.hpp"
#include "systems/language_system.hpp"

namespace tactile::cmd {

RenameLayer::RenameLayer(const Entity layer_entity, String name)
    : mLayerEntity {layer_entity},
      mNewName {std::move(name)}
{
}

void RenameLayer::undo()
{
  auto& model = get_global_model();

  auto& layer_context = model.get<Context>(mLayerEntity);
  layer_context.name = mOldName.value();

  mOldName.reset();
}

void RenameLayer::redo()
{
  auto& model = get_global_model();
  auto& layer_context = model.get<Context>(mLayerEntity);

  mOldName = layer_context.name;
  layer_context.name = mNewName;
}

auto RenameLayer::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(get_global_model());
  return strings.cmd.rename_layer;
}

}  // namespace tactile::cmd
