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

#include "document_delegate.hpp"

#include <utility>  // move

#include "misc/assert.hpp"

namespace tactile {

DocumentDelegate::DocumentDelegate(const UUID& rootContext)
    : mContexts {rootContext}
    , mComponentIndex {std::make_shared<ComponentIndex>()}
{}

void DocumentDelegate::set_component_index(Shared<ComponentIndex> index)
{
  mComponentIndex = std::move(index);
}

void DocumentDelegate::set_path(fs::path path)
{
  mPath = std::move(path);
}

auto DocumentDelegate::get_component_index() -> const Shared<ComponentIndex>&
{
  return mComponentIndex;
}

auto DocumentDelegate::view_component_index() const -> const ComponentIndex*
{
  return mComponentIndex.get();
}

auto DocumentDelegate::path() const -> const fs::path&
{
  TACTILE_ASSERT(mPath.has_value());
  return mPath.value();
}

auto DocumentDelegate::has_path() const -> bool
{
  return mPath.has_value();
}

auto DocumentDelegate::get_contexts() -> ContextManager&
{
  return mContexts;
}

auto DocumentDelegate::get_contexts() const -> const ContextManager&
{
  return mContexts;
}

auto DocumentDelegate::get_history() -> CommandStack&
{
  return mCommands;
}

auto DocumentDelegate::get_history() const -> const CommandStack&
{
  return mCommands;
}

auto DocumentDelegate::get_viewport() -> Viewport&
{
  return mViewport;
}

auto DocumentDelegate::get_viewport() const -> const Viewport&
{
  return mViewport;
}

}  // namespace tactile