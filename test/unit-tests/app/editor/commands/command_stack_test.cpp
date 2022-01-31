#include "editor/commands/command_stack.hpp"

#include <iostream>  // cout

#include <gtest/gtest.h>

#include "io/preferences.hpp"

using namespace tactile;

namespace {

class Foo : public ACommand {
 public:
  Foo() : ACommand{"Foo"} {}

  void Undo() override
  {
    // std::cout << "Foo::Undo\n";
  }

  void Redo() override
  {
    // std::cout << "Foo::Redo\n";
  }

  [[nodiscard]] auto GetId() const -> int override { return 0; }
};

class Bar : public ACommand {
 public:
  Bar() : ACommand{"Bar"} {}

  void Undo() override
  {
    // std::cout << "Bar::Undo\n";
  }

  void Redo() override
  {
    // std::cout << "Bar::Redo\n";
  }

  [[nodiscard]] auto GetId() const -> int override { return 1; }
};

}  // namespace

TEST(CommandStack, Defaults)
{
  const CommandStack stack;
  ASSERT_EQ(0, stack.GetSize());
  ASSERT_EQ(get_preferences().command_capacity(), stack.GetCapacity());
  ASSERT_FALSE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());
  ASSERT_TRUE(stack.IsClean());
}

TEST(CommandStack, Usage)
{
  CommandStack stack;

  // ^[ ] -> [ ^Foo ]
  stack.Push<Foo>();
  ASSERT_EQ(1, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());
  ASSERT_EQ("Foo", stack.GetUndoText());

  // [ ^Foo ] -> [ Foo, ^Bar ]
  stack.Push<Bar>();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());
  ASSERT_EQ("Bar", stack.GetUndoText());

  // [ Foo, ^Bar ] -> [ ^Foo, Bar ]
  stack.Undo();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_TRUE(stack.CanRedo());
  ASSERT_EQ("Foo", stack.GetUndoText());
  ASSERT_EQ("Bar", stack.GetRedoText());

  // [ ^Foo, Bar ] -> ^[ Foo, Bar ]
  stack.Undo();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_FALSE(stack.CanUndo());
  ASSERT_TRUE(stack.CanRedo());
  ASSERT_EQ("Foo", stack.GetRedoText());

  // ^[ Foo, Bar ] -> [ ^Foo, Bar ]
  stack.Redo();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_TRUE(stack.CanRedo());
  ASSERT_EQ("Foo", stack.GetUndoText());
  ASSERT_EQ("Bar", stack.GetRedoText());

  // [ ^Foo, Bar ] -> [ Foo, ^Bar ]
  stack.Redo();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());
  ASSERT_EQ("Bar", stack.GetUndoText());

  // [ Foo, ^Bar ] -> [ ^Foo, Bar ]
  stack.Undo();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_TRUE(stack.CanRedo());
  ASSERT_EQ("Foo", stack.GetUndoText());
  ASSERT_EQ("Bar", stack.GetRedoText());

  // [ ^Foo, Bar ] -> [ Foo, ^Foo ]
  stack.Push<Foo>();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());
  ASSERT_EQ("Foo", stack.GetUndoText());

  // [ Foo, ^Foo ] -> ^[ Foo, Foo ]
  stack.Undo();
  stack.Undo();
  ASSERT_EQ(2, stack.GetSize());
  ASSERT_EQ("Foo", stack.GetRedoText());

  // ^[ Foo, Foo ] -> [ ^Bar ]
  stack.Push<Bar>();
  ASSERT_EQ(1, stack.GetSize());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());
  ASSERT_EQ("Bar", stack.GetUndoText());
}

TEST(CommandStack, Clean)
{
  CommandStack stack;

  ASSERT_TRUE(stack.IsClean());
  ASSERT_FALSE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());

  stack.MarkAsClean();
  stack.Push<Foo>();
  ASSERT_FALSE(stack.IsClean());

  stack.Undo();
  ASSERT_TRUE(stack.IsClean());

  // ^[ ] -> [ ^Foo ]
  stack.Clear();
  stack.ResetClean();
  stack.Push<Foo>();
  ASSERT_FALSE(stack.IsClean());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());

  // [ ^Foo ] -> [ Foo, ^Bar ]
  stack.Push<Bar>();
  ASSERT_FALSE(stack.IsClean());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());

  stack.MarkAsClean();
  ASSERT_TRUE(stack.IsClean());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());

  // [ Foo, ^Bar ] -> [ ^Foo, Bar ]
  stack.Undo();
  ASSERT_FALSE(stack.IsClean());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_TRUE(stack.CanRedo());

  // [ ^Foo, Bar ] -> [ Foo, ^Bar ]
  stack.Redo();
  ASSERT_TRUE(stack.IsClean());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());

  /* Here we test a special case when the clean state becomes invalidated */
  // [ Foo, ^Bar ] -Undo-> [ ^Foo, Bar ] -Push-> [ Foo, ^Foo ]
  stack.Undo();
  stack.Push<Foo>();
  ASSERT_FALSE(stack.IsClean());
  ASSERT_TRUE(stack.CanUndo());
  ASSERT_FALSE(stack.CanRedo());

  stack.MarkAsClean();
  ASSERT_TRUE(stack.IsClean());

  stack.ResetClean();
  ASSERT_FALSE(stack.IsClean());
}

TEST(CommandStack, OverflowWithCleanIndex)
{
  CommandStack stack;

  stack.SetCapacity(4);
  ASSERT_EQ(4, stack.GetCapacity());

  stack.Push<Foo>();

  stack.Push<Bar>();
  stack.MarkAsClean();

  stack.Push<Bar>();
  stack.Push<Bar>();

  //  ^[ ] -> [ Foo, _Bar_, Bar, ^Bar ]
  ASSERT_EQ(4, stack.GetSize());
  ASSERT_EQ(3, stack.GetIndex());
  ASSERT_EQ(1, stack.GetCleanIndex());
  ASSERT_EQ("Bar", stack.GetUndoText());

  // [ Foo, _Bar_, Bar, ^Bar ] -> [ _Bar_, Bar, Bar, ^Foo ]
  stack.Push<Foo>();
  ASSERT_EQ(4, stack.GetSize());
  ASSERT_EQ(3, stack.GetIndex());
  ASSERT_EQ(0, stack.GetCleanIndex());
  ASSERT_EQ("Foo", stack.GetUndoText());

  // [ _Bar_, Bar, Bar, ^Foo ] -> [ Bar, Bar, Foo, ^Bar ]
  stack.Push<Bar>();
  ASSERT_EQ(4, stack.GetSize());
  ASSERT_EQ(3, stack.GetIndex());
  ASSERT_FALSE(stack.GetCleanIndex());
  ASSERT_EQ("Bar", stack.GetUndoText());
}

TEST(CommandStack, Overflow)
{
  CommandStack stack;
  stack.Push<Foo>();

  ASSERT_EQ(1, stack.GetSize());
  ASSERT_EQ(0, stack.GetIndex());
  ASSERT_EQ("Foo", stack.GetUndoText());

  // The stack should be full after this
  for (auto index = 0u; index < (stack.GetCapacity() - 1); ++index) {
    stack.Push<Bar>();
  }

  ASSERT_EQ(stack.GetCapacity(), stack.GetSize());
  ASSERT_EQ(stack.GetCapacity() - 1, stack.GetIndex());
  ASSERT_EQ("Bar", stack.GetUndoText());

  stack.Push<Foo>();  // This should cause the first command to get removed
  ASSERT_EQ(stack.GetCapacity(), stack.GetSize());
}

TEST(CommandStack, SetCapacity)
{
  CommandStack stack;

  stack.SetCapacity(5);
  ASSERT_EQ(5, stack.GetCapacity());

  for (auto index = 0u; index < 5; ++index) {
    stack.Push<Foo>();
  }
  ASSERT_EQ(5, stack.GetSize());
  ASSERT_EQ("Foo", stack.GetUndoText());

  // [ Foo, Foo, Foo, Foo, ^Foo ] -> [ Foo, Foo, Foo, Foo, ^Bar ]
  stack.Push<Bar>();
  ASSERT_EQ(5, stack.GetSize());
  ASSERT_EQ(4, stack.GetIndex());
  ASSERT_EQ("Bar", stack.GetUndoText());

  // [ Foo, Foo, Foo, Foo, ^Bar ] -> [ Foo, Foo, ^Bar ]
  stack.SetCapacity(3);
  ASSERT_EQ(3, stack.GetCapacity());
  ASSERT_EQ(3, stack.GetSize());
  ASSERT_EQ(2, stack.GetIndex());
  ASSERT_EQ("Bar", stack.GetUndoText());

  stack.SetCapacity(10);
  ASSERT_EQ(10, stack.GetCapacity());
  ASSERT_EQ(3, stack.GetSize());
  ASSERT_EQ(2, stack.GetIndex());
  ASSERT_EQ("Bar", stack.GetUndoText());
}