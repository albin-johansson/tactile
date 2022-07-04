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

#include "undef_component_cmd.hpp"

#include <utility>  // move

#include "core/components/component_index.hpp"

namespace tactile {

UndefComponentCmd::UndefComponentCmd(Shared<core::ComponentIndex> index,
                                     const UUID&                  componentId)
    : mIndex{std::move(index)}
    , mComponentId{componentId}
{}

void UndefComponentCmd::undo()
{
  const auto& previous = mPrevious.value();

  mIndex->define_comp(mComponentId, previous.get_name());
  auto& definition = mIndex->at(mComponentId);

  for (const auto& [key, value] : previous) {
    definition.add_attr(key, value);
  }

  mPrevious.reset();
}

void UndefComponentCmd::redo()
{
  mPrevious = mIndex->at(mComponentId);
  mIndex->remove_comp(mComponentId);
}

auto UndefComponentCmd::get_name() const -> const char*
{
  return "Remove Component Definition";
}

}  // namespace tactile