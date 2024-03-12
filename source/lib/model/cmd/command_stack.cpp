// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "command_stack.hpp"

#include "common/util/functional.hpp"
#include "model/settings.hpp"
#include "tactile/core/debug/assert.hpp"

namespace tactile {

CommandStack::CommandStack()
    : mCapacity {get_settings().get_command_capacity()}
{
}

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

auto CommandStack::get_undo_text() const -> String
{
  TACTILE_ASSERT(can_undo());

  const auto& cmd = mStack.at(mIndex.value());
  return cmd->get_name();
}

auto CommandStack::get_redo_text() const -> String
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
