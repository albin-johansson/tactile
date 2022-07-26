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

#include "command_stack.hpp"

#include "core/common/functional.hpp"
#include "io/persistence/preferences.hpp"
#include "misc/assert.hpp"

namespace tactile {

CommandStack::CommandStack()
    : mCapacity {io::get_preferences().command_capacity}
{}

void CommandStack::clear()
{
  mStack.clear();
  mIndex.reset();
}

void CommandStack::mark_as_clean()
{
  mCleanIndex = mIndex;
}

void CommandStack::reset_clean()
{
  mCleanIndex.reset();
}

void CommandStack::undo()
{
  TACTILE_ASSERT(can_undo());

  const auto& cmd = mStack.at(mIndex.value());
  cmd->undo();

  if (mIndex == 0) {
    mIndex.reset();
  }
  else {
    mIndex = mIndex.value() - 1;
  }
}

void CommandStack::redo()
{
  TACTILE_ASSERT(can_redo());

  const auto index = mIndex ? *mIndex + 1 : 0;

  const auto& cmd = mStack.at(index);
  cmd->redo();

  mIndex = index;
}

void CommandStack::set_capacity(const usize capacity)
{
  mCapacity = capacity;

  const auto count = size();
  if (count > mCapacity) {
    const auto n = count - mCapacity;
    invoke_n(n, [this] { remove_oldest_command(); });
  }
}

auto CommandStack::is_clean() const -> bool
{
  return mStack.empty() || (mCleanIndex == mIndex);
}

auto CommandStack::can_undo() const -> bool
{
  return !mStack.empty() && mIndex.has_value();
}

auto CommandStack::can_redo() const -> bool
{
  return (!mStack.empty() && !mIndex) || (!mStack.empty() && mIndex < mStack.size() - 1);
}

auto CommandStack::get_undo_text() const -> std::string
{
  TACTILE_ASSERT(can_undo());

  const auto& cmd = mStack.at(mIndex.value());
  return cmd->get_name();
}

auto CommandStack::get_redo_text() const -> std::string
{
  TACTILE_ASSERT(can_redo());

  const auto& cmd = mStack.at(mIndex ? *mIndex + 1 : 0);
  return cmd->get_name();
}

void CommandStack::remove_oldest_command()
{
  mStack.pop_front();

  if (mIndex) {
    mIndex = *mIndex - 1;
  }

  if (mCleanIndex) {
    if (mCleanIndex == 0) {
      mCleanIndex.reset();
    }
    else {
      mCleanIndex = *mCleanIndex - 1;
    }
  }
}

void CommandStack::remove_commands_after_current_index()
{
  const auto startIndex = mIndex ? *mIndex + 1 : 0;
  const auto size = mStack.size();

  /* If we have a clean index, and there are undone commands when another
     command is pushed, then the clean index becomes invalidated */
  if (mCleanIndex >= startIndex) {
    mCleanIndex.reset();
  }

  for (auto index = startIndex; index < size; ++index) {
    mStack.pop_back();
  }
}

}  // namespace tactile
