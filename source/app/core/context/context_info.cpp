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

#include "context_info.hpp"

#include <utility>  // move

namespace tactile {

ContextInfo::ContextInfo(const UUID& id)
    : mId {id}
{
}

void ContextInfo::set_name(String name)
{
  mName = std::move(name);
}

auto ContextInfo::clone() const -> ContextInfo
{
  ContextInfo other {*this};
  other.mId = make_uuid();
  return other;
}

}  // namespace tactile