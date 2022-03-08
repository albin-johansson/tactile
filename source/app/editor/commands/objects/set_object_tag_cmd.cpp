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

#include "set_object_tag_cmd.hpp"

#include <utility>  // move

#include "core/components/object.hpp"

namespace tactile {

set_object_tag_cmd::set_object_tag_cmd(registry_ref registry,
                                       const object_id id,
                                       std::string tag)
    : object_cmd{"Set Object Tag", registry, id}
    , mNewTag{std::move(tag)}
{}

void set_object_tag_cmd::undo()
{
  auto& object = target_object();
  object.tag = mOldTag.value();
}

void set_object_tag_cmd::redo()
{
  auto& object = target_object();
  mOldTag = object.tag;
  object.tag = mNewTag;
}

auto set_object_tag_cmd::merge_with(const command_base& cmd) -> bool
{
  if (id() == cmd.id()) {
    const auto& other = dynamic_cast<const set_object_tag_cmd&>(cmd);
    if (target_object_id() == other.target_object_id()) {
      mNewTag = other.mNewTag;
      return true;
    }
  }

  return false;
}

}  // namespace tactile
