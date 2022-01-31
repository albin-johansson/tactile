#include "editor/commands/command_stack.hpp"

#include <iostream>  // cout

#include <gtest/gtest.h>

#include "io/persistence/preferences.hpp"

using namespace tactile;

namespace {

class Foo : public command_base {
 public:
  Foo() : command_base{"Foo"} {}

  void undo() override
  {
    // std::cout << "Foo::Undo\n";
  }

  void redo() override
  {
    // std::cout << "Foo::Redo\n";
  }

  [[nodiscard]] auto id() const -> int override { return 0; }
};

class Bar : public command_base {
 public:
  Bar() : command_base{"Bar"} {}

  void undo() override
  {
    // std::cout << "Bar::Undo\n";
  }

  void redo() override
  {
    // std::cout << "Bar::Redo\n";
  }

  [[nodiscard]] auto id() const -> int override { return 1; }
};

}  // namespace

TEST(CommandStack, Defaults)
{
  const command_stack stack;
  ASSERT_EQ(0, stack.size());
  ASSERT_EQ(get_preferences().command_capacity(), stack.capacity());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_TRUE(stack.is_clean());
}

TEST(CommandStack, Usage)
{
  command_stack stack;

  // ^[ ] -> [ ^Foo ]
  stack.push<Foo>();
  ASSERT_EQ(1, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("Foo", stack.get_undo_text());

  // [ ^Foo ] -> [ Foo, ^Bar ]
  stack.push<Bar>();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("Bar", stack.get_undo_text());

  // [ Foo, ^Bar ] -> [ ^Foo, Bar ]
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("Foo", stack.get_undo_text());
  ASSERT_EQ("Bar", stack.get_redo_text());

  // [ ^Foo, Bar ] -> ^[ Foo, Bar ]
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("Foo", stack.get_redo_text());

  // ^[ Foo, Bar ] -> [ ^Foo, Bar ]
  stack.redo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("Foo", stack.get_undo_text());
  ASSERT_EQ("Bar", stack.get_redo_text());

  // [ ^Foo, Bar ] -> [ Foo, ^Bar ]
  stack.redo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("Bar", stack.get_undo_text());

  // [ Foo, ^Bar ] -> [ ^Foo, Bar ]
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("Foo", stack.get_undo_text());
  ASSERT_EQ("Bar", stack.get_redo_text());

  // [ ^Foo, Bar ] -> [ Foo, ^Foo ]
  stack.push<Foo>();
  ASSERT_EQ(2, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("Foo", stack.get_undo_text());

  // [ Foo, ^Foo ] -> ^[ Foo, Foo ]
  stack.undo();
  stack.undo();
  ASSERT_EQ(2, stack.size());
  ASSERT_EQ("Foo", stack.get_redo_text());

  // ^[ Foo, Foo ] -> [ ^Bar ]
  stack.push<Bar>();
  ASSERT_EQ(1, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("Bar", stack.get_undo_text());
}

TEST(CommandStack, Clean)
{
  command_stack stack;

  ASSERT_TRUE(stack.is_clean());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  stack.push<Foo>();
  ASSERT_FALSE(stack.is_clean());

  stack.undo();
  ASSERT_TRUE(stack.is_clean());

  // ^[ ] -> [ ^Foo ]
  stack.clear();
  stack.reset_clean();
  stack.push<Foo>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // [ ^Foo ] -> [ Foo, ^Bar ]
  stack.push<Bar>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  ASSERT_TRUE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // [ Foo, ^Bar ] -> [ ^Foo, Bar ]
  stack.undo();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());

  // [ ^Foo, Bar ] -> [ Foo, ^Bar ]
  stack.redo();
  ASSERT_TRUE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  /* Here we test a special case when the clean state becomes invalidated */
  // [ Foo, ^Bar ] -Undo-> [ ^Foo, Bar ] -Push-> [ Foo, ^Foo ]
  stack.undo();
  stack.push<Foo>();
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
  command_stack stack;

  stack.set_capacity(4);
  ASSERT_EQ(4, stack.capacity());

  stack.push<Foo>();

  stack.push<Bar>();
  stack.mark_as_clean();

  stack.push<Bar>();
  stack.push<Bar>();

  //  ^[ ] -> [ Foo, _Bar_, Bar, ^Bar ]
  ASSERT_EQ(4, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_EQ(1, stack.clean_index());
  ASSERT_EQ("Bar", stack.get_undo_text());

  // [ Foo, _Bar_, Bar, ^Bar ] -> [ _Bar_, Bar, Bar, ^Foo ]
  stack.push<Foo>();
  ASSERT_EQ(4, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_EQ(0, stack.clean_index());
  ASSERT_EQ("Foo", stack.get_undo_text());

  // [ _Bar_, Bar, Bar, ^Foo ] -> [ Bar, Bar, Foo, ^Bar ]
  stack.push<Bar>();
  ASSERT_EQ(4, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_FALSE(stack.clean_index());
  ASSERT_EQ("Bar", stack.get_undo_text());
}

TEST(CommandStack, Overflow)
{
  command_stack stack;
  stack.push<Foo>();

  ASSERT_EQ(1, stack.size());
  ASSERT_EQ(0, stack.index());
  ASSERT_EQ("Foo", stack.get_undo_text());

  // The stack should be full after this
  for (auto index = 0u; index < (stack.capacity() - 1); ++index) {
    stack.push<Bar>();
  }

  ASSERT_EQ(stack.capacity(), stack.size());
  ASSERT_EQ(stack.capacity() - 1, stack.index());
  ASSERT_EQ("Bar", stack.get_undo_text());

  stack.push<Foo>();  // This should cause the first command to get removed
  ASSERT_EQ(stack.capacity(), stack.size());
}

TEST(CommandStack, SetCapacity)
{
  command_stack stack;

  stack.set_capacity(5);
  ASSERT_EQ(5, stack.capacity());

  for (auto index = 0u; index < 5; ++index) {
    stack.push<Foo>();
  }
  ASSERT_EQ(5, stack.size());
  ASSERT_EQ("Foo", stack.get_undo_text());

  // [ Foo, Foo, Foo, Foo, ^Foo ] -> [ Foo, Foo, Foo, Foo, ^Bar ]
  stack.push<Bar>();
  ASSERT_EQ(5, stack.size());
  ASSERT_EQ(4, stack.index());
  ASSERT_EQ("Bar", stack.get_undo_text());

  // [ Foo, Foo, Foo, Foo, ^Bar ] -> [ Foo, Foo, ^Bar ]
  stack.set_capacity(3);
  ASSERT_EQ(3, stack.capacity());
  ASSERT_EQ(3, stack.size());
  ASSERT_EQ(2, stack.index());
  ASSERT_EQ("Bar", stack.get_undo_text());

  stack.set_capacity(10);
  ASSERT_EQ(10, stack.capacity());
  ASSERT_EQ(3, stack.size());
  ASSERT_EQ(2, stack.index());
  ASSERT_EQ("Bar", stack.get_undo_text());
}