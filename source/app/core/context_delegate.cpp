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

#include "context_delegate.hpp"

#include <utility>  // move

namespace tactile::core {

void ContextDelegate::set_name(std::string name)
{
  mName = std::move(name);
}

auto ContextDelegate::get_props() -> PropertyBundle&
{
  return mProps;
}

auto ContextDelegate::get_props() const -> const PropertyBundle&
{
  return mProps;
}

auto ContextDelegate::get_comps() -> ComponentBundle&
{
  return mComps;
}

auto ContextDelegate::get_comps() const -> const ComponentBundle&
{
  return mComps;
}

auto ContextDelegate::get_uuid() const -> const UUID&
{
  return mId;
}

auto ContextDelegate::get_name() const -> const std::string&
{
  return mName;
}

auto ContextDelegate::clone() const -> ContextDelegate
{
  ContextDelegate other;
  other.mName = mName;
  other.mProps = mProps;
  other.mComps = mComps;
  return other;
}

}  // namespace tactile::core
