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

#include "add_row.hpp"

#include "common/util/functional.hpp"
#include "model/i18n/language_system.hpp"
#include "model/maps/map_components.hpp"
#include "model/maps/map_system.hpp"

namespace tactile::cmd {

AddRow::AddRow(Registry* registry, const Entity map_entity)
    : mRegistry {registry},
      mMapEntity {map_entity}
{
}

void AddRow::undo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  invoke_n(mRowCount, [&] { sys::remove_row_from_map(registry, map); });
}

void AddRow::redo()
{
  auto& registry = *mRegistry;

  auto& map = registry.get<Map>(mMapEntity);
  invoke_n(mRowCount, [&] { sys::add_row_to_map(registry, map); });
}

auto AddRow::merge_with(const Command* cmd) -> bool
{
  if (const auto* other = dynamic_cast<const AddRow*>(cmd)) {
    mRowCount += other->mRowCount;
    return true;
  }

  return false;
}

auto AddRow::get_name() const -> String
{
  const auto& strings = sys::get_current_language_strings(*mRegistry);
  return mRowCount == 1 ? strings.cmd.add_row : strings.cmd.add_rows;
}

}  // namespace tactile::cmd
