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

#include "set_object_visibility_cmd.hpp"

#include "core/components/objects.hpp"

namespace tactile {

SetObjectVisibilityCmd::SetObjectVisibilityCmd(RegistryRef registry,
                                               const ObjectID id,
                                               const bool visible)
    : AObjectCommand{"Set Object Visibility", registry, id}
    , mVisible{visible}
{}

void SetObjectVisibilityCmd::undo()
{
  auto& object = target_object();
  object.visible = mPreviousVisibility.value();
}

void SetObjectVisibilityCmd::redo()
{
  auto& object = target_object();
  mPreviousVisibility = object.visible;
  object.visible = mVisible;
}

}  // namespace tactile
