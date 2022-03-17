#include "editor/commands/command_stack.hpp"

#include <iostream>  // cout

#include <gtest/gtest.h>

#include "io/persistence/preferences.hpp"

using namespace tactile;

namespace {

struct foo_cmd : ACommand
{
  foo_cmd() : ACommand{"foo_cmd"} {}

  void undo() override
  {
    // std::cout << "foo_cmd::undo\n";
  }

  void redo() override
  {
    // std::cout << "foo_cmd::redo\n";
  }

  [[nodiscard]] auto id() const -> int override { return 0; }
};

struct bar_cmd : ACommand
{
  bar_cmd() : ACommand{"bar_cmd"} {}

  void undo() override
  {
    // std::cout << "bar_cmd::undo\n";
  }

  void redo() override
  {
    // std::cout << "bar_cmd::redo\n";
  }

  [[nodiscard]] auto id() const -> int override { return 1; }
};

}  // namespace

TEST(CommandStack, Defaults)
{
  const CommandStack stack;
  ASSERT_EQ(0, stack.size());
  ASSERT_EQ(get_preferences().command_capacity(), stack.capacity());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_TRUE(stack.is_clean());
}

TEST(CommandStack, Usage)
{
  CommandStack stack;

  // ^[ ] -> [ ^foo_cmd ]
  stack.push<foo_cmd>();
  ASSERT_EQ(1, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());

  // [ ^foo_cmd ] -> [ foo_cmd, ^bar_cmd ]
  stack.push<bar_cmd>();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());

  // [ foo_cmd, ^bar_cmd ] -> [ ^foo_cmd, bar_cmd ]
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());
  ASSERT_EQ("bar_cmd", stack.get_redo_text());

  // [ ^foo_cmd, bar_cmd ] -> ^[ foo_cmd, bar_cmd ]
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("foo_cmd", stack.get_redo_text());

  // ^[ foo_cmd, bar_cmd ] -> [ ^foo_cmd, bar_cmd ]
  stack.redo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());
  ASSERT_EQ("bar_cmd", stack.get_redo_text());

  // [ ^foo_cmd, bar_cmd ] -> [ foo_cmd, ^bar_cmd ]
  stack.redo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());

  // [ foo_cmd, ^bar_cmd ] -> [ ^foo_cmd, bar_cmd ]
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());
  ASSERT_EQ("bar_cmd", stack.get_redo_text());

  // [ ^foo_cmd, bar_cmd ] -> [ foo_cmd, ^foo_cmd ]
  stack.push<foo_cmd>();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());

  // [ foo_cmd, ^foo_cmd ] -> ^[ foo_cmd, foo_cmd ]
  stack.undo();
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_EQ("foo_cmd", stack.get_redo_text());

  // ^[ foo_cmd, foo_cmd ] -> [ ^bar_cmd ]
  stack.push<bar_cmd>();
  ASSERT_EQ(1, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());
}

TEST(CommandStack, Clean)
{
  CommandStack stack;

  ASSERT_TRUE(stack.is_clean());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  stack.push<foo_cmd>();
  ASSERT_FALSE(stack.is_clean());

  stack.undo();
  ASSERT_TRUE(stack.is_clean());

  // ^[ ] -> [ ^foo_cmd ]
  stack.clear();
  stack.reset_clean();
  stack.push<foo_cmd>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // [ ^foo_cmd ] -> [ foo_cmd, ^bar_cmd ]
  stack.push<bar_cmd>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  ASSERT_TRUE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // [ foo_cmd, ^bar_cmd ] -> [ ^foo_cmd, bar_cmd ]
  stack.undo();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());

  // [ ^foo_cmd, bar_cmd ] -> [ foo_cmd, ^bar_cmd ]
  stack.redo();
  ASSERT_TRUE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  /* Here we test a special case when the clean state becomes invalidated */
  // [ foo_cmd, ^bar_cmd ] -Undo-> [ ^foo_cmd, bar_cmd ] -Push-> [ foo_cmd, ^foo_cmd ]
  stack.undo();
  stack.push<foo_cmd>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  ASSERT_TRUE(stack.is_clean());

  stack.reset_clean();
  ASSERT_FALSE(stack.is_clean());
}

TEST(CommandStack, OverflowWithCleanIndex)
{
  CommandStack stack;

  stack.set_capacity(4);
  ASSERT_EQ(4, stack.capacity());

  stack.push<foo_cmd>();

  stack.push<bar_cmd>();
  stack.mark_as_clean();

  stack.push<bar_cmd>();
  stack.push<bar_cmd>();

  //  ^[ ] -> [ foo_cmd, _Bar_, bar_cmd, ^bar_cmd ]
  ASSERT_EQ(4, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_EQ(1, stack.clean_index());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());

  // [ foo_cmd, _Bar_, bar_cmd, ^bar_cmd ] -> [ _Bar_, bar_cmd, bar_cmd, ^foo_cmd ]
  stack.push<foo_cmd>();
  ASSERT_EQ(4, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_EQ(0, stack.clean_index());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());

  // [ _Bar_, bar_cmd, bar_cmd, ^foo_cmd ] -> [ bar_cmd, bar_cmd, foo_cmd, ^bar_cmd ]
  stack.push<bar_cmd>();
  ASSERT_EQ(4, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_FALSE(stack.clean_index());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());
}

TEST(CommandStack, Overflow)
{
  CommandStack stack;
  stack.push<foo_cmd>();

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(0, stack.index());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());

  // The stack should be full after this
  for (auto index = 0u; index < (stack.capacity() - 1); ++index) {
    stack.push<bar_cmd>();
  }

  ASSERT_EQ(stack.capacity(), stack.size());
  ASSERT_EQ(stack.capacity() - 1, stack.index());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());

  stack.push<foo_cmd>();  // This should cause the first command to get removed
  ASSERT_EQ(stack.capacity(), stack.size());
}

TEST(CommandStack, SetCapacity)
{
  CommandStack stack;

  stack.set_capacity(5);
  ASSERT_EQ(5, stack.capacity());

  for (auto index = 0u; index < 5; ++index) {
    stack.push<foo_cmd>();
  }
  ASSERT_EQ(5, stack.size());
  ASSERT_EQ("foo_cmd", stack.get_undo_text());

  // [ foo_cmd, foo_cmd, foo_cmd, foo_cmd, ^foo_cmd ] -> [ foo_cmd, foo_cmd, foo_cmd, foo_cmd, ^bar_cmd ]
  stack.push<bar_cmd>();
  ASSERT_EQ(5, stack.size());
  ASSERT_EQ(4, stack.index());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());

  // [ foo_cmd, foo_cmd, foo_cmd, foo_cmd, ^bar_cmd ] -> [ foo_cmd, foo_cmd, ^bar_cmd ]
  stack.set_capacity(3);
  ASSERT_EQ(3, stack.capacity());
  ASSERT_EQ(3, stack.size());
  ASSERT_EQ(2, stack.index());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());

  stack.set_capacity(10);
  ASSERT_EQ(10, stack.capacity());
  ASSERT_EQ(3, stack.size());
  ASSERT_EQ(2, stack.index());
  ASSERT_EQ("bar_cmd", stack.get_undo_text());
}