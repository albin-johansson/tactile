// Copyright (C) 2025 Albin Johansson

#include <gtest/gtest.h>

import tactile.editor.command;
import tactile.core.prelude;

namespace tactile::editor::tests {
namespace {

struct TestCommand : ICommand
{
  explicit TestCommand(const StringView name)
    : name {name}
  {}

  void redo() override
  {
    ++redo_calls;
  }

  void undo() override
  {
    ++undo_calls;
  }

  StringView name;
  usize redo_calls {0};
  usize undo_calls {0};
};

struct MergableTestCommand final : TestCommand
{
  using TestCommand::TestCommand;

  [[nodiscard]]
  auto merge(const ICommand& other) -> bool override
  {
    const auto* that = dynamic_cast<const MergableTestCommand*>(&other);
    if (that == nullptr) {
      return false;
    }

    count += that->count;
    return true;
  }

  usize count {1};
};

class CommandStackTest : public testing::Test
{
 protected:
  CommandStack m_stack {10};

  [[nodiscard]]
  auto get_cursor_command() const -> const TestCommand*
  {
    return dynamic_cast<const TestCommand*>(m_stack.cursor_command());
  }

  [[nodiscard]]
  auto cursor_command_is(const StringView name) const -> bool
  {
    const auto* command = get_cursor_command();
    return command != nullptr && command->name == name;
  }
};

TEST_F(CommandStackTest, Defaults)
{
  EXPECT_EQ(m_stack.size(), 0);
  EXPECT_EQ(m_stack.capacity(), 10);
  EXPECT_EQ(m_stack.cursor(), kNone);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_FALSE(m_stack.can_undo());
  EXPECT_FALSE(m_stack.can_redo());
}

TEST_F(CommandStackTest, InvalidCapacity)
{
  EXPECT_THROW(CommandStack {0}, std::invalid_argument);
}

TEST_F(CommandStackTest, Execute)
{
  // [ ] -> [ >A ]
  m_stack.execute<TestCommand>("A");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_TRUE(m_stack.can_undo());
  EXPECT_FALSE(m_stack.can_redo());

  // [ >A ] -> [ A, >B ]
  m_stack.execute<TestCommand>("B");

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.cursor(), 1);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(cursor_command_is("B"));
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_TRUE(m_stack.can_undo());
  EXPECT_FALSE(m_stack.can_redo());

  // [ A, >B ] -> [ A, B, >C ]
  m_stack.execute<TestCommand>("C");

  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 2);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(cursor_command_is("C"));
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_TRUE(m_stack.can_undo());
  EXPECT_FALSE(m_stack.can_redo());
}

TEST_F(CommandStackTest, ExecuteWithMergableCommands)
{
  // [ ] -> [ >_A_ ]
  m_stack.execute<MergableTestCommand>("A");
  m_stack.mark_as_clean();

  ASSERT_TRUE(cursor_command_is("A"));
  ASSERT_EQ(m_stack.size(), 1);
  ASSERT_EQ(m_stack.cursor(), 0);
  ASSERT_EQ(m_stack.clean_cursor(), 0);
  ASSERT_FALSE(m_stack.is_dirty());

  {
    const auto* command =
        dynamic_cast<const MergableTestCommand*>(m_stack.cursor_command());
    ASSERT_NE(command, nullptr);
    EXPECT_EQ(command->count, 1);
  }

  // [ >_A_ ] -> [ >A ]
  m_stack.execute<MergableTestCommand>("B");

  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());

  {
    const auto* command =
        dynamic_cast<const MergableTestCommand*>(m_stack.cursor_command());
    ASSERT_NE(command, nullptr);
    EXPECT_EQ(command->count, 2);
  }

  // [ >A ] -> [ A, >C ]
  m_stack.execute<TestCommand>("C");

  EXPECT_TRUE(cursor_command_is("C"));
  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.cursor(), 1);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());
}

TEST_F(CommandStackTest, ExecuteShouldRemoveRevertedCommands)
{
  // [ ] -> [ A, B, >C ]
  m_stack.execute<TestCommand>("A");
  m_stack.execute<TestCommand>("B");
  m_stack.execute<TestCommand>("C");
  ASSERT_TRUE(cursor_command_is("C"));
  ASSERT_EQ(m_stack.size(), 3);
  ASSERT_EQ(m_stack.cursor(), 2);

  // [ ] -> [ A, >B, C ]
  m_stack.undo();
  EXPECT_TRUE(cursor_command_is("B"));
  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 1);

  // [ A, >B, C ] -> [ A, B, >D ]
  m_stack.execute<TestCommand>("D");
  EXPECT_TRUE(cursor_command_is("D"));
  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 2);

  // [ A, B, >D ] -> [ A, >B, D ]
  m_stack.undo();
  EXPECT_TRUE(cursor_command_is("B"));
  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 1);
}

TEST_F(CommandStackTest, ExecuteShouldRemoveOldestCommandWhenStackIsFull)
{
  const auto capacity = m_stack.capacity();

  for (auto index = 0uz; index < capacity; ++index) {
    m_stack.execute<TestCommand>("A");
  }
  ASSERT_TRUE(cursor_command_is("A"));
  ASSERT_EQ(m_stack.size(), capacity);

  m_stack.execute<TestCommand>("B");
  EXPECT_TRUE(cursor_command_is("B"));
  EXPECT_EQ(m_stack.size(), capacity);
}

TEST_F(CommandStackTest,
       ExecuteShouldResetCleanCursorIfRevertedCleanCommandIsRemoved)
{
  // [ ] -> [ A, >_B_ ]
  m_stack.execute<TestCommand>("A");
  m_stack.execute<TestCommand>("B");
  m_stack.mark_as_clean();

  ASSERT_EQ(m_stack.size(), 2);
  ASSERT_EQ(m_stack.cursor(), 1);
  ASSERT_EQ(m_stack.clean_cursor(), 1);
  ASSERT_FALSE(m_stack.is_dirty());

  // [ A, >_B_ ] -> [ >A, _B_ ]
  m_stack.undo();

  ASSERT_EQ(m_stack.size(), 2);
  ASSERT_EQ(m_stack.cursor(), 0);
  ASSERT_EQ(m_stack.clean_cursor(), 1);
  ASSERT_TRUE(m_stack.is_dirty());
  ASSERT_TRUE(cursor_command_is("A"));

  // [ >A, _B_ ] -> [ A, >C ]
  m_stack.execute<TestCommand>("C");

  EXPECT_EQ(m_stack.size(), 2);
  EXPECT_EQ(m_stack.cursor(), 1);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_TRUE(cursor_command_is("C"));
}

TEST_F(CommandStackTest,
       ExecuteShouldResetCleanCursorIfCommandIsMergedWithCleanCommand)
{
  // [ ] -> [ >_A_ ]
  m_stack.execute<MergableTestCommand>("A");
  m_stack.mark_as_clean();

  ASSERT_EQ(m_stack.size(), 1);
  ASSERT_EQ(m_stack.cursor(), 0);
  ASSERT_EQ(m_stack.clean_cursor(), 0);
  ASSERT_FALSE(m_stack.is_dirty());
  ASSERT_TRUE(cursor_command_is("A"));

  // [ >_A_ ] -> [ >A ]
  m_stack.execute<MergableTestCommand>("B");

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_TRUE(cursor_command_is("A"));

  // [ >A ] -> [ A ]
  m_stack.undo();

  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), kNone);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());
  EXPECT_EQ(m_stack.cursor_command(), nullptr);
}

TEST_F(CommandStackTest, ExecuteShouldKeepCleanCursorIfOldCleanCommandIsRemoved)
{
  // [ ] -> [ >_A_ ]
  m_stack.execute<TestCommand>("A");
  m_stack.mark_as_clean();

  ASSERT_EQ(m_stack.size(), 1);
  ASSERT_EQ(m_stack.capacity(), 10);
  ASSERT_EQ(m_stack.cursor(), 0);
  ASSERT_EQ(m_stack.clean_cursor(), 0);
  ASSERT_FALSE(m_stack.is_dirty());
  ASSERT_TRUE(cursor_command_is("A"));

  // [ >_A_ ] -> [ _A_, B, C, D, E, F, G, H, I, >J ]
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
  EXPECT_EQ(m_stack.capacity(), 10);
  EXPECT_EQ(m_stack.cursor(), 9);
  EXPECT_EQ(m_stack.clean_cursor(), 0);
  EXPECT_TRUE(m_stack.is_dirty());
  ASSERT_TRUE(cursor_command_is("J"));

  // [ _A_, B, C, D, E, F, G, H, I, >J ] -> [ _B_, C, D, E, F, G, H, I, J, >K ]
  m_stack.execute<TestCommand>("K");

  ASSERT_EQ(m_stack.size(), 10);
  ASSERT_EQ(m_stack.capacity(), 10);
  ASSERT_EQ(m_stack.cursor(), 9);
  ASSERT_EQ(m_stack.clean_cursor(), 0);
  ASSERT_TRUE(m_stack.is_dirty());
  ASSERT_TRUE(cursor_command_is("K"));

  // [ _B_, C, D, E, F, G, H, I, J, >K ] -> [ _B_, C, D, E, F, G, H, I, >J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("J"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, D, E, F, G, H, I, >J, K ] -> [ _B_, C, D, E, F, G, H, >I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("I"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, D, E, F, G, H, >I, J, K ] -> [ _B_, C, D, E, F, G, >H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("H"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, D, E, F, G, >H, I, J, K ] -> [ _B_, C, D, E, F, >G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("G"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, D, E, F, >G, H, I, J, K ] -> [ _B_, C, D, E, >F, G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("F"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, D, E, >F, G, H, I, J, K ] -> [ _B_, C, D, >E, F, G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("E"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, D, >E, F, G, H, I, J, K ] -> [ _B_, C, >D, E, F, G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("D"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, C, >D, E, F, G, H, I, J, K ] -> [ _B_, >C, D, E, F, G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("C"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, >C, D, E, F, G, H, I, J, K ] -> [ >_B_, C, D, E, F, G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("B"));
  EXPECT_FALSE(m_stack.is_dirty());

  // [ >_B_, C, D, E, F, G, H, I, J, K ] -> [ _B_, >C, D, E, F, G, H, I, J, K ]
  m_stack.redo();
  ASSERT_TRUE(cursor_command_is("C"));
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _B_, >C, D, E, F, G, H, I, J, K ] -> [ >_B_, C, D, E, F, G, H, I, J, K ]
  m_stack.undo();
  ASSERT_TRUE(cursor_command_is("B"));
  EXPECT_FALSE(m_stack.is_dirty());
}

TEST_F(CommandStackTest, Store)
{
  m_stack.store(make_unique<TestCommand>("A"));
  EXPECT_EQ(m_stack.size(), 1);

  const auto* cursor_command = get_cursor_command();
  ASSERT_NE(cursor_command, nullptr);

  EXPECT_EQ(cursor_command->name, "A");
  EXPECT_EQ(cursor_command->redo_calls, 0);
  EXPECT_EQ(cursor_command->undo_calls, 0);
}

TEST_F(CommandStackTest, UndoRedoSimple)
{
  // [ ] -> [ >A ]
  m_stack.execute<TestCommand>("A");

  {
    const auto* cursor_command = get_cursor_command();
    ASSERT_NE(cursor_command, nullptr);
    EXPECT_EQ(cursor_command->name, "A");
    EXPECT_EQ(cursor_command->redo_calls, 1);
    EXPECT_EQ(cursor_command->undo_calls, 0);
  }

  // [ >A ] -> [ A ]
  m_stack.undo();
  EXPECT_EQ(m_stack.cursor_command(), nullptr);

  // [ A ] -> [ >A ]
  m_stack.redo();

  {
    const auto* cursor_command = get_cursor_command();
    ASSERT_NE(cursor_command, nullptr);
    EXPECT_EQ(cursor_command->name, "A");
    EXPECT_EQ(cursor_command->redo_calls, 2);
    EXPECT_EQ(cursor_command->undo_calls, 1);
  }
}

TEST_F(CommandStackTest, UndoRedoAdvanced)
{
  // [ ] -> [ A, >B ]
  m_stack.execute<TestCommand>("A");
  m_stack.execute<TestCommand>("B");

  ASSERT_TRUE(cursor_command_is("B"));
  ASSERT_TRUE(m_stack.can_undo());
  ASSERT_FALSE(m_stack.can_redo());
  ASSERT_EQ(m_stack.cursor(), 1);
  ASSERT_EQ(m_stack.size(), 2);

  // [ A, >B ] -> [ >A, B ]
  m_stack.undo();

  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_TRUE(m_stack.can_undo());
  EXPECT_TRUE(m_stack.can_redo());
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.size(), 2);

  // [ >A, B ] -> [ A, B ]
  m_stack.undo();

  EXPECT_EQ(m_stack.cursor_command(), nullptr);
  EXPECT_FALSE(m_stack.can_undo());
  EXPECT_TRUE(m_stack.can_redo());
  EXPECT_EQ(m_stack.cursor(), kNone);
  EXPECT_EQ(m_stack.size(), 2);

  // [ A, B ] -> [ >A, B ]
  m_stack.redo();

  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_TRUE(m_stack.can_undo());
  EXPECT_TRUE(m_stack.can_redo());
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.size(), 2);

  // [ >A, B ] -> [ A, >B ]
  m_stack.redo();

  EXPECT_TRUE(cursor_command_is("B"));
  EXPECT_TRUE(m_stack.can_undo());
  EXPECT_FALSE(m_stack.can_redo());
  EXPECT_EQ(m_stack.cursor(), 1);
  EXPECT_EQ(m_stack.size(), 2);
}

TEST_F(CommandStackTest, MarkAsClean)
{
  ASSERT_EQ(m_stack.size(), 0);
  ASSERT_EQ(m_stack.cursor(), kNone);
  ASSERT_EQ(m_stack.clean_cursor(), kNone);
  ASSERT_TRUE(m_stack.is_dirty());

  m_stack.mark_as_clean();
  EXPECT_EQ(m_stack.size(), 0);
  EXPECT_EQ(m_stack.cursor(), kNone);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_FALSE(m_stack.is_dirty());

  // [ __ ] -> [ >A ]
  m_stack.execute<MergableTestCommand>("A");
  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());

  // [ >A ] -> [ __ ]
  m_stack.undo();
  EXPECT_EQ(m_stack.cursor_command(), nullptr);
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), kNone);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_FALSE(m_stack.is_dirty());

  // [ __ ] -> [ >A ]
  m_stack.execute<MergableTestCommand>("A");
  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());

  // [ >A ] -> [ >_A_ ]
  m_stack.mark_as_clean();
  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), 0);
  EXPECT_FALSE(m_stack.is_dirty());

  // [ >_A_ ] -> [ _A_, B, >C ]
  m_stack.execute<TestCommand>("B");
  m_stack.execute<TestCommand>("C");
  EXPECT_TRUE(cursor_command_is("C"));
  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 2);
  EXPECT_EQ(m_stack.clean_cursor(), 0);
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _A_, B, >C ] -> [ _A_, >B, C ]
  m_stack.undo();
  EXPECT_TRUE(cursor_command_is("B"));
  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 1);
  EXPECT_EQ(m_stack.clean_cursor(), 0);
  EXPECT_TRUE(m_stack.is_dirty());

  // [ _A_, >B, C ] -> [ >_A_, B, C ]
  m_stack.undo();
  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_EQ(m_stack.size(), 3);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), 0);
  EXPECT_FALSE(m_stack.is_dirty());

  // [ >_A_, B, C ] -> [ >A ]
  m_stack.execute<MergableTestCommand>("D");
  EXPECT_TRUE(cursor_command_is("A"));
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), 0);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());

  // [ >A ] -> [ A ]
  m_stack.undo();
  EXPECT_EQ(m_stack.cursor_command(), nullptr);
  EXPECT_EQ(m_stack.size(), 1);
  EXPECT_EQ(m_stack.cursor(), kNone);
  EXPECT_EQ(m_stack.clean_cursor(), kNone);
  EXPECT_TRUE(m_stack.is_dirty());
}

}  // namespace
}  // namespace tactile::editor::tests
