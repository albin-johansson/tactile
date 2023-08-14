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

#include "command_stack.hpp"

#include "core/debug/assert.hpp"
#include "core/functional/invoke.hpp"

namespace tactile {

CommandStack::CommandStack(const usize capacity)
    : mCapacity {capacity}
{
}

void CommandStack::clear()
{
  mStack.clear();
  mCurrentIndex.reset();
}

void CommandStack::mark_as_clean()
{
  mCleanIndex = mCurrentIndex;
}

void CommandStack::reset_clean()
{
  mCleanIndex.reset();
}

void CommandStack::undo()
{
  TACTILE_ASSERT(can_undo());

  mStack.at(mCurrentIndex.value())->undo();

  if (mCurrentIndex == 0) {
    mCurrentIndex.reset();
  }
  else {
    mCurrentIndex = *mCurrentIndex - 1;
  }
}

void CommandStack::redo()
{
  TACTILE_ASSERT(can_redo());

  const auto index = _get_next_index();

  mStack.at(index)->redo();
  mCurrentIndex = index;
}

void CommandStack::set_capacity(const usize capacity)
{
  mCapacity = capacity;

  const auto count = size();
  if (count > mCapacity) {
    const auto n = count - mCapacity;
    invoke_n(n, [this] { _remove_oldest_command(); });
  }
}

auto CommandStack::is_clean() const -> bool
{
  return mStack.empty() || (mCleanIndex == mCurrentIndex);
}

auto CommandStack::can_undo() const -> bool
{
  return !mStack.empty() && mCurrentIndex.has_value();
}

auto CommandStack::can_redo() const -> bool
{
  return (!mStack.empty() && !mCurrentIndex.has_value()) ||
         (!mStack.empty() && mCurrentIndex < mStack.size() - 1);
}

auto CommandStack::get_undo_text() const -> String
{
  TACTILE_ASSERT(can_undo());
  return mStack.at(mCurrentIndex.value())->get_name();
}

auto CommandStack::get_redo_text() const -> String
{
  TACTILE_ASSERT(can_redo());
  return mStack.at(_get_next_index())->get_name();
}

void CommandStack::_store(UniquePtr<Command> cmd)
{
  if (size() == capacity()) {
    _remove_oldest_command();
  }

  _remove_commands_after_current_index();

  mCurrentIndex = _get_next_index();
  mStack.push_back(std::move(cmd));
}

void CommandStack::_remove_oldest_command()
{
  TACTILE_ASSERT(!mStack.empty());

  mStack.front()->dispose();
  mStack.pop_front();

  if (mCurrentIndex.has_value()) {
    mCurrentIndex = *mCurrentIndex - 1;
  }

  if (mCleanIndex.has_value()) {
    if (mCleanIndex == 0) {
      mCleanIndex.reset();
    }
    else {
      mCleanIndex = *mCleanIndex - 1;
    }
  }
}

void CommandStack::_remove_commands_after_current_index()
{
  const auto start_index = _get_next_index();

  // If we have a clean index and there are undone commands when another
  // command is pushed, then the clean index becomes invalidated.
  if (mCleanIndex >= start_index) {
    mCleanIndex.reset();
  }

  const auto command_count = mStack.size();
  for (auto index = start_index; index < command_count; ++index) {
    mStack.back()->dispose();
    mStack.pop_back();
  }
}

auto CommandStack::_get_next_index() const -> usize
{
  return mCurrentIndex.has_value() ? (*mCurrentIndex + 1) : 0;
}

}  // namespace tactile
