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

#include "move_layer_down.hpp"

#include "common/debug/assert.hpp"
#include "model/entity_validation.hpp"
#include "model/i18n/language_system.hpp"
#include "model/layers/layer_tree_system.hpp"
#include "model/maps/map_components.hpp"

namespace tactile::cmd {

MoveLayerDown::MoveLayerDown(Model* model,
                             const Entity map_entity,
                             const Entity layer_entity)
    : mModel {model},
      mMapEntity {map_entity},
      mLayerEntity {layer_entity}
{
  TACTILE_ASSERT(sys::is_map_entity(*mModel, mMapEntity));
  TACTILE_ASSERT(sys::is_layer_entity(*mModel, mLayerEntity));
}

void MoveLayerDown::undo()
{
  auto& model = *mModel;

  const auto& map = model.get<Map>(mMapEntity);
  sys::move_layer_up(model, map, mLayerEntity);
}

void MoveLayerDown::redo()
{
  auto& model = *mModel;

  const auto& map = model.get<Map>(mMapEntity);
  sys::move_layer_down(model, map, mLayerEntity);
}

auto MoveLayerDown::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.move_layer_down;
}

}  // namespace tactile::cmd
