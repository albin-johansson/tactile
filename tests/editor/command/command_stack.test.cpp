// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "tactile/editor/command/command_stack.hpp"

#include <stdexcept>
#include <utility>

#include <gtest/gtest.h>

#include "tactile/core/container/string.hpp"

namespace tactile::editor {
namespace {

class TestCommand : public ICommand
{
 public:
  explicit TestCommand(String name)
    : m_name {std::move(name)}
  {}

  void redo() override
  {
    ++m_redo_count;
  }

  void undo() override
  {
    ++m_undo_count;
  }

  String m_name {};
  usize m_redo_count {0};
  usize m_undo_count {0};
};

class MergableTestCommand final : public TestCommand
{
 public:
  using TestCommand::TestCommand;

  auto merge(const ICommand& other) -> bool override
  {
    const auto* cmd = dynamic_cast<const MergableTestCommand*>(&other);
    if (cmd == nullptr) {
      return false;
    }

    // Update the name to make diagrams easier to understand.
    m_name = cmd->m_name;
    ++m_merge_count;

    return true;
  }

  usize m_merge_count {0};
};

// These tests use simple diagrams to illustrate the expected command stack state. The
// following rules explain the syntax.
//
// - The stack is displayed as a bracketed array, with commands added left-to-right. In
//   other words, the oldest command is shown as the leftmost command.
// - A caret prefix is used to denote the cursor command.
// - An exclamation point suffix is used to denote the clean cursor command.
//
// For example, consider the following diagram.
//
//    [ A, ^B, C! ]
//
// From this, we can determine that the stack should contain three commands, with
// B as the current cursor command, and C as the clean cursor command.
class CommandStackTest : public testing::Test
{
 public:
  auto cursor_command_is(const StringView name) const -> bool
  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    return cursor != nullptr && cursor->m_name == name;
  }

 protected:
  CommandStack m_stack {10};
};

TEST_F(CommandStackTest, Defaults)
{
  EXPECT_EQ(m_stack.capacity(), 10);
  EXPECT_EQ(m_stack.size(), 0);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), false);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);
}

TEST_F(CommandStackTest, InvalidCapacity)
{
  EXPECT_THROW(CommandStack {0}, std::invalid_argument);
  EXPECT_THROW(CommandStack {-1}, std::invalid_argument);
}

TEST_F(CommandStackTest, MarkAsCleanWhenEmpty)
{
  EXPECT_EQ(m_stack.is_clean(), false);
  m_stack.mark_as_clean();
  EXPECT_EQ(m_stack.is_clean(), true);
}

TEST_F(CommandStackTest, MarkAsClean)
{
  // ^[ ] => ^![ ]
  m_stack.mark_as_clean();

  EXPECT_EQ(m_stack.size(), 0);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), false);

  // ^![ ] => ![ ^A ]
  m_stack.execute<MergableTestCommand>("A");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 0);
  }

  // ![ ^A ] => ^![ A ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);

  // ^![ A ] => [ ^B ]
  m_stack.execute<MergableTestCommand>("B");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 0);
  }

  // [ ^B ] => [ ^B! ]
  m_stack.mark_as_clean();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 0);
  }

  // [ ^B! ] => [ B!, C, ^D ]
  m_stack.execute<TestCommand>("C");
  m_stack.execute<TestCommand>("D");

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "D");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ B!, C, ^D ] => [ B!, ^C, D ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "C");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ B!, ^C, D ] => [ ^B!, C, D ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 0);
  }

  // [ ^B!, C, D ] => [ ^E ]
  m_stack.execute<MergableTestCommand>("E");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "E");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 1);
  }

  // [ ^E ] => ^[ E ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);
}

TEST_F(CommandStackTest, UndoWhenEmpty)
{
  EXPECT_THROW(m_stack.undo(), std::runtime_error);
}

TEST_F(CommandStackTest, RedoWhenEmpty)
{
  EXPECT_THROW(m_stack.redo(), std::runtime_error);
}

TEST_F(CommandStackTest, StoreNullCommand)
{
  EXPECT_THROW(m_stack.store(nullptr), std::invalid_argument);
}

TEST_F(CommandStackTest, Store)
{
  // ^[ ] => [ ^A ]
  m_stack.store(make_unique<TestCommand>("A"));

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
  ASSERT_NE(cursor, nullptr);
  EXPECT_EQ(cursor->m_name, "A");
  EXPECT_EQ(cursor->m_redo_count, 0);
  EXPECT_EQ(cursor->m_undo_count, 0);
}

TEST_F(CommandStackTest, Execute)
{
  // ^[ ] => [ ^A ]
  m_stack.execute<TestCommand>("A");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ ^A ] => [ A, ^B ]
  m_stack.execute<TestCommand>("B");

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ A, ^B ] => [ A, B, ^C ]
  m_stack.execute<TestCommand>("C");

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "C");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }
}

TEST_F(CommandStackTest, ExecuteWithMergableCommands)
{
  // ^[ ] => [ ^A! ]
  m_stack.execute<MergableTestCommand>("A");
  m_stack.mark_as_clean();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 0);
  }

  // [ ^A! ] => [ ^B ]
  m_stack.execute<MergableTestCommand>("B");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 1);
  }

  // [ ^B ] => [ B, ^C ]
  m_stack.execute<TestCommand>("C");

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "C");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }
}

TEST_F(CommandStackTest, ExecuteShouldRemoveRevertedCommands)
{
  // ^[ ] => [ A, B, ^C ]
  m_stack.execute<TestCommand>("A");
  m_stack.execute<TestCommand>("B");
  m_stack.execute<TestCommand>("C");

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "C");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ A, B, ^C ] => [ A, ^B, C ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ A, ^B, C ] => [ A, B, ^D ]
  m_stack.execute<TestCommand>("D");

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "D");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ A, B, ^D ] => [ A, ^B, D ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }
}

TEST_F(CommandStackTest, ExecuteShouldRemoveOldestCommandWhenFull)
{
  const auto capacity = m_stack.capacity();

  for (auto index = 0z; index < capacity; ++index) {
    m_stack.execute<TestCommand>("A");
  }

  EXPECT_EQ(m_stack.size(), capacity);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  m_stack.execute<TestCommand>("B");

  EXPECT_EQ(m_stack.size(), capacity);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }
}

TEST_F(CommandStackTest, ExecuteShouldResetCleanStateIfRevertedCleanCommandIsRemoved)
{
  // ^[ ] => [ A, ^B! ]
  m_stack.execute<TestCommand>("A");
  m_stack.execute<TestCommand>("B");
  m_stack.mark_as_clean();

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ A, ^B! ] => [ ^A, B! ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ ^A, B! ] => [ A, ^C ]
  m_stack.execute<TestCommand>("C");

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "C");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }
}

TEST_F(CommandStackTest, ExecuteShouldResetCleanStateIfCleanCommandIsUpdatedWithMerge)
{
  // ^[ ] => [ ^A! ]
  m_stack.execute<MergableTestCommand>("A");
  m_stack.mark_as_clean();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 0);
  }

  // [ ^A! ] => [ ^B ]
  m_stack.execute<MergableTestCommand>("B");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor =
        dynamic_cast<const MergableTestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
    EXPECT_EQ(cursor->m_merge_count, 1);
  }

  // [ ^B ] => ^[ B ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);
}

TEST_F(CommandStackTest,
       ExecuteShouldKeepCleanStateIfOldCleanCommandIsAutomaticallyRemoved)
{
  ASSERT_EQ(m_stack.capacity(), 10);

  // ^[ ] => [ ^A! ]
  m_stack.execute<TestCommand>("A");
  m_stack.mark_as_clean();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);
  EXPECT_TRUE(cursor_command_is("A"));

  // [ ^A! ] => [ A!, B, C, D, E, F, G, H, I, ^J ]
  m_stack.execute<TestCommand>("B");
  m_stack.execute<TestCommand>("C");
  m_stack.execute<TestCommand>("D");
  m_stack.execute<TestCommand>("E");
  m_stack.execute<TestCommand>("F");
  m_stack.execute<TestCommand>("G");
  m_stack.execute<TestCommand>("H");
  m_stack.execute<TestCommand>("I");
  m_stack.execute<TestCommand>("J");

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);
  EXPECT_TRUE(cursor_command_is("J"));

  // [ A!, B, C, D, E, F, G, H, I, ^J ] => [ B!, C, D, E, F, G, H, I, J, ^K ]
  m_stack.execute<TestCommand>("K");

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);
  EXPECT_TRUE(cursor_command_is("K"));

  // [ B!, C, D, E, F, G, H, I, J, ^K ] => [ B!, C, D, E, F, G, H, I, ^J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("J"));

  // [ B!, C, D, E, F, G, H, I, ^J, K ] => [ B!, C, D, E, F, G, H, ^I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("I"));

  // [ B!, C, D, E, F, G, H, ^I, J, K ] => [ B!, C, D, E, F, G, ^H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("H"));

  // [ B!, C, D, E, F, G, ^H, I, J, K ] => [ B!, C, D, E, F, ^G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("G"));

  // [ B!, C, D, E, F, ^G, H, I, J, K ] => [ B!, C, D, E, ^F, G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("F"));

  // [ B!, C, D, E, ^F, G, H, I, J, K ] => [ B!, C, D, ^E, F, G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("E"));

  // [ B!, C, D, ^E, F, G, H, I, J, K ] => [ B!, C, ^D, E, F, G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("D"));

  // [ B!, C, ^D, E, F, G, H, I, J, K ] => [ B!, ^C, D, E, F, G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("C"));

  // [ B!, ^C, D, E, F, G, H, I, J, K ] => [ ^B!, C, D, E, F, G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), true);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_TRUE(cursor_command_is("B"));

  // [ ^B!, C, D, E, F, G, H, I, J, K ] => ^[ B!, C, D, E, F, G, H, I, J, K ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 10);
  EXPECT_EQ(m_stack.size(), m_stack.capacity());
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);
}

TEST_F(CommandStackTest, ExecuteUndoRedoSimple)
{
  // ^[ ] => [ ^A ]
  m_stack.execute<TestCommand>("A");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ ^A ] => ^[ A ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);

  // ^[ A ] => [ ^A ]
  m_stack.redo();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 2);
    EXPECT_EQ(cursor->m_undo_count, 1);
  }
}

TEST_F(CommandStackTest, ExecuteUndoRedoAdvanced)
{
  // ^[ ] => [ A, ^B ]
  m_stack.execute<TestCommand>("A");
  m_stack.execute<TestCommand>("B");

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ A, ^B ] => [ ^A, B ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 1);
    EXPECT_EQ(cursor->m_undo_count, 0);
  }

  // [ ^A, B ] => ^[ A, B ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), false);
  EXPECT_EQ(m_stack.can_redo(), true);
  EXPECT_EQ(m_stack.get_cursor_command(), nullptr);

  // ^[ A, B ] => [ ^A, B ]
  m_stack.redo();

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), true);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "A");
    EXPECT_EQ(cursor->m_redo_count, 2);
    EXPECT_EQ(cursor->m_undo_count, 1);
  }

  // [ ^A, B ] => [ A, ^B ]
  m_stack.redo();

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.is_clean(), false);
  EXPECT_EQ(m_stack.can_undo(), true);
  EXPECT_EQ(m_stack.can_redo(), false);

  {
    const auto* cursor = dynamic_cast<const TestCommand*>(m_stack.get_cursor_command());
    ASSERT_NE(cursor, nullptr);
    EXPECT_EQ(cursor->m_name, "B");
    EXPECT_EQ(cursor->m_redo_count, 2);
    EXPECT_EQ(cursor->m_undo_count, 1);
  }
}

}  // namespace
}  // namespace tactile::editor
