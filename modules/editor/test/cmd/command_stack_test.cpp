// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/editor/cmd/command_stack.hpp"

#include <gtest/gtest.h>

using namespace tactile;
using namespace tactile::editor;
using namespace tactile::int_literals;

namespace {

inline constexpr uint32 kCmd1 = 1;
inline constexpr uint32 kCmd2 = 2;

class C1 final : public ICommand {
 public:
  void undo() override {}
  void redo() override {}
  [[nodiscard]] auto get_id() const -> uint32 override { return kCmd1; }
};

class C2 final : public ICommand {
 public:
  void undo() override {}
  void redo() override {}
  [[nodiscard]] auto get_id() const -> uint32 override { return kCmd2; }
};

}  // namespace

TEST(CommandStack, Constructor)
{
  const CommandStack command_stack {64};

  EXPECT_EQ(command_stack.get_size(), 0_uz);
  EXPECT_EQ(command_stack.get_capacity(), 64_uz);
  EXPECT_TRUE(command_stack.is_clean());
  EXPECT_FALSE(command_stack.can_undo());
  EXPECT_FALSE(command_stack.can_redo());
  EXPECT_FALSE(command_stack.get_index().has_value());
  EXPECT_FALSE(command_stack.get_clean_index().has_value());
}

TEST(CommandStack, MixedUsage)
{
  CommandStack stack {64};
  EXPECT_EQ(stack.get_capacity(), 64_uz);

  // [] -push-> [ current:C1 ]
  stack.push<C1>();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 1_uz);
  EXPECT_EQ(stack.get_index(), 0_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_FALSE(stack.can_redo());

  // [ current:C1 ] -push-> [ C1, current:C2 ]
  stack.push<C2>();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 2_uz);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_FALSE(stack.can_redo());

  // [ C1, current:C2 ] -undo-> [ current:C1, C2 ]
  stack.undo();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 2_uz);
  EXPECT_EQ(stack.get_index(), 0_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_TRUE(stack.can_redo());

  // [ current:C1, C2 ] -undo-> [ C1, C2 ]
  stack.undo();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 2_uz);
  EXPECT_EQ(stack.get_index(), kNone);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_FALSE(stack.can_undo());
  EXPECT_TRUE(stack.can_redo());

  // [ C1, C2 ] -redo-> [ current:C1, C2 ]
  stack.redo();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 2_uz);
  EXPECT_EQ(stack.get_index(), 0_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_TRUE(stack.can_redo());

  // [ current:C1, C2 ] -redo-> [ C1, current:C2 ]
  stack.redo();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 2_uz);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_FALSE(stack.can_redo());

  // [ C1, current:C2 ] -push-> [ C1, C2, current:C1 ]
  stack.push<C1>();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 3_uz);
  EXPECT_EQ(stack.get_index(), 2_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_FALSE(stack.can_redo());

  // [ C1, C2, current:C1 ] -undo x3-> [ C1, C2, C1 ]
  stack.undo();
  stack.undo();
  stack.undo();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 3_uz);
  EXPECT_EQ(stack.get_index(), kNone);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_FALSE(stack.can_undo());
  EXPECT_TRUE(stack.can_redo());

  // [ C1, C2, C1 ] -push-> [ current:C2 ]
  stack.push<C2>();
  EXPECT_EQ(stack.get_capacity(), 64_uz);
  EXPECT_EQ(stack.get_size(), 1_uz);
  EXPECT_EQ(stack.get_index(), 0_uz);
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.can_undo());
  EXPECT_FALSE(stack.can_redo());
}

TEST(CommandStack, CleanIndexManagement)
{
  CommandStack stack {32};

  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.is_clean());

  // This should be a no-op in this case.
  stack.mark_as_clean();
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.is_clean());

  // Adding a command should make the command stack dirty.
  stack.push<C1>();
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_FALSE(stack.is_clean());

  // No clean index is set yet, but this should make the stack clean again.
  stack.undo();
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_TRUE(stack.is_clean());

  // [] -push-> [ current:C1 ] -push-> [ C1, current:C2 ]
  stack.push<C1>();
  stack.push<C2>();
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_FALSE(stack.is_clean());

  // [ C1, current:C2 ] -mark-as-clean-> [ C1, current|clean:C2 ]
  stack.mark_as_clean();
  EXPECT_EQ(stack.get_clean_index(), 1_uz);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_TRUE(stack.is_clean());

  // [ C1, current|clean:C2 ] -undo-> [ current:C1, clean:C2 ]
  stack.undo();
  EXPECT_EQ(stack.get_clean_index(), 1_uz);
  EXPECT_EQ(stack.get_index(), 0_uz);
  EXPECT_FALSE(stack.is_clean());

  // [ current:C1, clean:C2 ] -redo-> [ C1, current|clean:C2 ]
  stack.redo();
  EXPECT_EQ(stack.get_clean_index(), 1_uz);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_TRUE(stack.is_clean());

  // Special case when the clean state becomes invalidated.
  // [ C1, current|clean:C2 ] -undo-> [ current:C1, clean:C2 ] -push-> [ C1, current:C1 ]
  stack.undo();
  stack.push<C1>();
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_FALSE(stack.is_clean());

  // [ C1, current:C1 ] -mark-as-clean-> [ C1, current|clean:C1 ]
  stack.mark_as_clean();
  EXPECT_EQ(stack.get_clean_index(), 1_uz);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_TRUE(stack.is_clean());

  // [ C1, current|clean:C1 ] -reset-clean-index-> [ C1, current:C1 ]
  stack.reset_clean_index();
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_EQ(stack.get_index(), 1_uz);
  EXPECT_FALSE(stack.is_clean());
}

TEST(CommandStack, CommandOverflowWithDefinedCleanIndex)
{
  CommandStack stack {4};
  EXPECT_EQ(stack.get_capacity(), 4_uz);

  // [] -push-> [ current:C1 ] -push-> [ C1, current:C2 ]
  stack.push<C1>();
  stack.push<C2>();

  // [ C1, current:C2 ] -mark-as-clean-> [ C1: current|clean:C2 ]
  stack.mark_as_clean();

  // [ C1: current|clean:C2 ] -push-> [ C1: clean:C2, current:C1 ]
  stack.push<C1>();

  // [ C1: clean:C2, current:C1 ] -push-> [ C1: clean:C2, C1, current:C2 ]
  stack.push<C2>();

  EXPECT_EQ(stack.get_size(), stack.get_capacity());
  EXPECT_EQ(stack.get_clean_index(), 1_uz);
  EXPECT_EQ(stack.get_index(), 3_uz);
  EXPECT_FALSE(stack.is_clean());

  // [ C1: clean:C2, C1, current:C2 ] -push-> [ clean:C2, C1, C2, current:C1 ]
  stack.push<C1>();

  EXPECT_EQ(stack.get_size(), stack.get_capacity());
  EXPECT_EQ(stack.get_clean_index(), 0_uz);
  EXPECT_EQ(stack.get_index(), 3_uz);
  EXPECT_FALSE(stack.is_clean());

  // [ clean:C2, C1, C2, current:C1 ] -push-> [ C1, C2, C1, current:C2 ]
  stack.push<C2>();

  EXPECT_EQ(stack.get_size(), stack.get_capacity());
  EXPECT_EQ(stack.get_clean_index(), kNone);
  EXPECT_EQ(stack.get_index(), 3_uz);
  EXPECT_FALSE(stack.is_clean());
}

TEST(CommandStack, SimpleCommandOverflow)
{
  CommandStack stack {100};

  for (auto index = 0_uz; index < stack.get_capacity() + 10; ++index) {
    stack.push<C1>();
  }

  EXPECT_EQ(stack.get_size(), stack.get_capacity());
  EXPECT_EQ(stack.get_index(), stack.get_capacity() - 1);
}

TEST(CommandStack, DynamicCapacityChange)
{
  CommandStack stack {50};

  for (auto index = 0_uz; index < stack.get_capacity(); ++index) {
    stack.push<C1>();
  }

  EXPECT_EQ(stack.get_size(), stack.get_capacity());

  stack.set_capacity(20);
  EXPECT_EQ(stack.get_size(), 20_uz);
  EXPECT_EQ(stack.get_capacity(), 20_uz);

  stack.set_capacity(25);
  EXPECT_EQ(stack.get_size(), 20_uz);
  EXPECT_EQ(stack.get_capacity(), 25_uz);
}
