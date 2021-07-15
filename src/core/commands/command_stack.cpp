#include "command_stack.hpp"

#include <cassert>  // assert

namespace Tactile {

void CommandStack::Clear()
{
  mStack.clear();
  mIndex.reset();
}

void CommandStack::MarkAsClean()
{
  mCleanIndex = mIndex;
}

void CommandStack::ResetClean()
{
  mCleanIndex.reset();
}

void CommandStack::Undo()
{
  assert(CanUndo());

  const auto& cmd = mStack.at(mIndex.value());
  cmd->Undo();

  if (mIndex == 0)
  {
    mIndex.reset();
  }
  else
  {
    mIndex = mIndex.value() - 1;
  }
}

void CommandStack::Redo()
{
  assert(CanRedo());

  const auto index = mIndex ? *mIndex + 1 : 0;

  const auto& cmd = mStack.at(index);
  cmd->Redo();

  mIndex = index;
}

auto CommandStack::IsClean() const -> bool
{
  return mStack.empty() || (mCleanIndex && mCleanIndex == mIndex);
}

auto CommandStack::CanUndo() const -> bool
{
  return !mStack.empty() && mIndex.has_value();
}

auto CommandStack::CanRedo() const -> bool
{
  return (!mStack.empty() && !mIndex) ||
         (!mStack.empty() && mIndex < mStack.size() - 1);
}

auto CommandStack::GetUndoText() const -> const std::string&
{
  assert(CanUndo());

  const auto& cmd = mStack.at(mIndex.value());
  return cmd->GetText();
}

auto CommandStack::GetRedoText() const -> const std::string&
{
  assert(CanRedo());

  const auto& cmd = mStack.at(mIndex ? *mIndex + 1 : 0);
  return cmd->GetText();
}

void CommandStack::RemoveCommandsAfterCurrentIndex()
{
  const auto startIndex = mIndex ? *mIndex + 1 : 0;
  const auto size = mStack.size();

  /* If we have a clean index, and there are undone commands when another
     command is pushed, then the clean index becomes invalidated */
  if (mCleanIndex >= startIndex)
  {
    mCleanIndex.reset();
  }

  for (auto index = startIndex; index < size; ++index)
  {
    mStack.pop_back();
  }
}

}  // namespace Tactile
