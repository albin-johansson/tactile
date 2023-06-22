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

#include "set_zstd_compression_level.hpp"

#include "model/maps/map_components.hpp"
#include "model/systems/language_system.hpp"
#include "model/tiles/tile_components.hpp"

namespace tactile::cmd {

SetZstdCompressionLevel::SetZstdCompressionLevel(Model* model,
                                                 const Entity map_entity,
                                                 const int new_level)
    : mModel {model},
      mMapEntity {map_entity},
      mNewLevel {new_level}
{
}

void SetZstdCompressionLevel::undo()
{
  auto& model = *mModel;

  auto& format = model.get<TileFormat>(mMapEntity);
  format.zstd_compression_level = mOldLevel.value();

  mOldLevel.reset();
}

void SetZstdCompressionLevel::redo()
{
  auto& model = *mModel;
  auto& format = model.get<TileFormat>(mMapEntity);

  mOldLevel = format.zstd_compression_level;
  format.zstd_compression_level = mNewLevel;
}

auto SetZstdCompressionLevel::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetZstdCompressionLevel*>(cmd)) {
    mNewLevel = other->mNewLevel;
    return true;
  }

  return false;
}

auto SetZstdCompressionLevel::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mModel);
  return strings.cmd.set_zstd_compression_level;
}

}  // namespace tactile::cmd