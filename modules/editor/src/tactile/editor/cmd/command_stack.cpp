// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/editor/cmd/command_stack.hpp"

#include "tactile/core/debug/assert.hpp"

namespace tactile::editor {

CommandStack::CommandStack(const usize capacity)
  : mCapacity {capacity}
{}

void CommandStack::clear()
{
  mCommands.clear();
  mCurrentIndex.reset();
}

void CommandStack::mark_as_clean()
{
  mCleanIndex = mCurrentIndex;
}

void CommandStack::reset_clean_index()
{
  mCleanIndex.reset();
}

void CommandStack::undo()
{
  TACTILE_ASSERT(can_undo());

  mCommands.at(mCurrentIndex.value())->undo();
  _reset_or_decrease_current_index();
}

void CommandStack::redo()
{
  TACTILE_ASSERT(can_redo());

  mCommands.at(_get_next_command_index())->redo();
  _increase_current_index();
}

void CommandStack::_store(Unique<ICommand> cmd)
{
  if (get_size() == get_capacity()) {
    _remove_oldest_command();
  }

  _remove_commands_after_current_index();
  _increase_current_index();

  mCommands.push_back(std::move(cmd));
}

void CommandStack::set_capacity(const usize capacity)
{
  using namespace int_literals;

  mCapacity = capacity;

  const auto command_count = get_size();
  if (command_count > mCapacity) {
    const auto excess_count = command_count - mCapacity;
    for (auto i = 0_uz; i < excess_count; ++i) {
      _remove_oldest_command();
    }
  }
}

auto CommandStack::is_clean() const -> bool
{
  return mCommands.empty() || (mCleanIndex == mCurrentIndex);
}

auto CommandStack::can_undo() const -> bool
{
  return !mCommands.empty() && mCurrentIndex.has_value();
}

auto CommandStack::can_redo() const -> bool
{
  return (!mCommands.empty() && !mCurrentIndex.has_value()) ||
         (!mCommands.empty() && mCurrentIndex < mCommands.size() - 1);
}

auto CommandStack::get_size() const -> usize
{
  return mCommands.size();
}

auto CommandStack::get_index() const -> Maybe<usize>
{
  return mCurrentIndex;
}

auto CommandStack::get_clean_index() const -> Maybe<usize>
{
  return mCleanIndex;
}

auto CommandStack::get_capacity() const -> usize
{
  return mCapacity;
}

void CommandStack::_remove_oldest_command()
{
  TACTILE_ASSERT(!mCommands.empty());

  mCommands.pop_front();
  _reset_or_decrease_current_index();
  _reset_or_decrease_clean_index();
}

void CommandStack::_remove_commands_after_current_index()
{
  const auto start_index = _get_next_command_index();

  // If we have a clean index, and there are undone commands when another command is
  // pushed, then the clean index becomes invalidated.
  if (mCleanIndex >= start_index) {
    mCleanIndex.reset();
  }

  const auto command_count = mCommands.size();
  for (auto cmd_index = start_index; cmd_index < command_count; ++cmd_index) {
    mCommands.pop_back();
  }
}

void CommandStack::_reset_or_decrease_clean_index()
{
  if (mCleanIndex.has_value()) {
    if (mCleanIndex == 0) {
      mCleanIndex = kNone;
    }
    else {
      mCleanIndex = *mCleanIndex - 1;
    }
  }
}

void CommandStack::_reset_or_decrease_current_index()
{
  if (mCurrentIndex.has_value()) {
    if (mCurrentIndex == 0) {
      mCurrentIndex = kNone;
    }
    else {
      mCurrentIndex = *mCurrentIndex - 1;
    }
  }
}

void CommandStack::_increase_current_index()
{
  mCurrentIndex = _get_next_command_index();
}

auto CommandStack::_get_next_command_index() const -> usize
{
  return mCurrentIndex.has_value() ? (*mCurrentIndex + 1) : 0;
}

}  // namespace tactile::editor
