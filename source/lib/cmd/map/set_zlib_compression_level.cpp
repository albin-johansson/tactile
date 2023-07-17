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

#include "set_zlib_compression_level.hpp"

#include "model/i18n/language_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/tiles/tile_components.hpp"

namespace tactile::cmd {

SetZlibCompressionLevel::SetZlibCompressionLevel(Registry* registry,
                                                 const Entity map_entity,
                                                 const int new_level)
    : mRegistry {registry},
      mMapEntity {map_entity},
      mNewLevel {new_level}
{
}

void SetZlibCompressionLevel::undo()
{
  auto& registry = *mRegistry;
  auto& format = registry.get<TileFormat>(mMapEntity);

  format.zlib_compression_level = mOldLevel.value();
  mOldLevel.reset();
}

void SetZlibCompressionLevel::redo()
{
  auto& registry = *mRegistry;
  auto& format = registry.get<TileFormat>(mMapEntity);

  mOldLevel = format.zlib_compression_level;
  format.zlib_compression_level = mNewLevel;
}

auto SetZlibCompressionLevel::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const SetZlibCompressionLevel*>(cmd)) {
    mNewLevel = other->mNewLevel;
    return true;
  }

  return false;
}

auto SetZlibCompressionLevel::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return strings.cmd.set_zlib_compression_level;
}

}  // namespace tactile::cmd
