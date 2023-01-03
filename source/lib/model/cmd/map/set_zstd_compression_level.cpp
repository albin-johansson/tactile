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

#include "set_zstd_compression_level.hpp"

#include <utility>  // move

#include "common/debug/panic.hpp"
#include "core/layer/tile_format.hpp"
#include "core/map.hpp"
#include "lang/language.hpp"
#include "lang/strings.hpp"

namespace tactile::cmd {

SetZstdCompressionLevel::SetZstdCompressionLevel(Shared<Map> map, const int level)
    : mMap {std::move(map)},
      mNewLevel {level}
{
  if (!mMap) {
    throw TactileError {"Invalid null map!"};
  }
}

void SetZstdCompressionLevel::undo()
{
  auto& format = mMap->tile_format();

  format.set_zstd_compression_level(mOldLevel.value());
  mOldLevel.reset();
}

void SetZstdCompressionLevel::redo()
{
  auto& format = mMap->tile_format();

  mOldLevel = format.zstd_compression_level();
  format.set_zstd_compression_level(mNewLevel);
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
  const auto& lang = get_current_language();
  return lang.cmd.set_zstd_compression_level;
}

}  // namespace tactile::cmd