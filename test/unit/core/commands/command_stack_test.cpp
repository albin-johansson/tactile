#include "core/commands/command_stack.hpp"

#include <gtest/gtest.h>

#include <iostream>  // cout

using namespace Tactile;

namespace {

class Foo : public ACommand
{
 public:
  Foo() : ACommand{"Foo"}
  {}

  void Undo() override
  {
    // std::cout << "Foo::Undo\n";
  }

  void Redo() override
  {
    // std::cout << "Foo::Redo\n";
  }

  [[nodiscard]] auto GetId() const -> int override
  {
    return 0;
  }
};

class Bar : public ACommand
{
 public:
  Bar() : ACommand{"Bar"}
  {}

  void Undo() override
  {
    std::cout << "Bar::Undo\n";
  }

  void Redo() override
  {
    std::cout << "Bar::Redo\n";
  }

  [[nodiscard]] auto GetId() const -> int override
  {
    return 1;
  }
};

}  // namespace

TEST(CommandStack, Defaults)
{
  const CommandStack stack;
  ASSERT_EQ(0, stack.GetSize());
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

  // ^[ ] -> [ ^Foo ]
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