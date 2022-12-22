/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2022 Albin Johansson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "model/cmd/command_stack.hpp"

#include <gtest/gtest.h>

#include "io/proto/preferences.hpp"

namespace tactile::test {
namespace {

struct FooCmd : Command {
  void undo() override {}

  void redo() override {}

  [[nodiscard]] auto get_name() const -> String override { return "FooCmd"; }
};

struct BarCmd : Command {
  void undo() override {}

  void redo() override {}

  [[nodiscard]] auto get_name() const -> String override { return "BarCmd"; }
};

}  // namespace

TEST(CommandStack, Defaults)
{
  const CommandStack stack;
  ASSERT_EQ(0u, stack.size());
  ASSERT_EQ(io::get_preferences().command_capacity, stack.capacity());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_TRUE(stack.is_clean());
}

TEST(CommandStack, Usage)
{
  CommandStack stack;

  // ^[ ] -> [ ^FooCmd ]
  stack.push<FooCmd>();
  ASSERT_EQ(1u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("FooCmd", stack.get_undo_text());

  // [ ^FooCmd ] -> [ FooCmd, ^BarCmd ]
  stack.push<BarCmd>();
  ASSERT_EQ(2u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("BarCmd", stack.get_undo_text());

  // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
  stack.undo();
  ASSERT_EQ(2u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("FooCmd", stack.get_undo_text());
  ASSERT_EQ("BarCmd", stack.get_redo_text());

  // [ ^FooCmd, BarCmd ] -> ^[ FooCmd, BarCmd ]
  stack.undo();
  ASSERT_EQ(2u, stack.size());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("FooCmd", stack.get_redo_text());

  // ^[ FooCmd, BarCmd ] -> [ ^FooCmd, BarCmd ]
  stack.redo();
  ASSERT_EQ(2u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("FooCmd", stack.get_undo_text());
  ASSERT_EQ("BarCmd", stack.get_redo_text());

  // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^BarCmd ]
  stack.redo();
  ASSERT_EQ(2u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("BarCmd", stack.get_undo_text());

  // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
  stack.undo();
  ASSERT_EQ(2u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());
  ASSERT_EQ("FooCmd", stack.get_undo_text());
  ASSERT_EQ("BarCmd", stack.get_redo_text());

  // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^FooCmd ]
  stack.push<FooCmd>();
  ASSERT_EQ(2u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("FooCmd", stack.get_undo_text());

  // [ FooCmd, ^FooCmd ] -> ^[ FooCmd, FooCmd ]
  stack.undo();
  stack.undo();
  ASSERT_EQ(2u, stack.size());
  ASSERT_EQ("FooCmd", stack.get_redo_text());

  // ^[ FooCmd, FooCmd ] -> [ ^BarCmd ]
  stack.push<BarCmd>();
  ASSERT_EQ(1u, stack.size());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());
  ASSERT_EQ("BarCmd", stack.get_undo_text());
}

TEST(CommandStack, Clean)
{
  CommandStack stack;

  ASSERT_TRUE(stack.is_clean());
  ASSERT_FALSE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  stack.push<FooCmd>();
  ASSERT_FALSE(stack.is_clean());

  stack.undo();
  ASSERT_TRUE(stack.is_clean());

  // ^[ ] -> [ ^FooCmd ]
  stack.clear();
  stack.reset_clean();
  stack.push<FooCmd>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // [ ^FooCmd ] -> [ FooCmd, ^BarCmd ]
  stack.push<BarCmd>();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  stack.mark_as_clean();
  ASSERT_TRUE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
  stack.undo();
  ASSERT_FALSE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_TRUE(stack.can_redo());

  // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^BarCmd ]
  stack.redo();
  ASSERT_TRUE(stack.is_clean());
  ASSERT_TRUE(stack.can_undo());
  ASSERT_FALSE(stack.can_redo());

  // Here we test a special case when the clean state becomes invalidated
  // [ FooCmd, ^BarCmd ] -Undo-> [ ^FooCmd, BarCmd ] -Push-> [ FooCmd, ^FooCmd ]
  stack.undo();
  stack.push<FooCmd>();
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
  ASSERT_EQ(4u, stack.capacity());

  stack.push<FooCmd>();

  stack.push<BarCmd>();
  stack.mark_as_clean();

  stack.push<BarCmd>();
  stack.push<BarCmd>();

  //  ^[ ] -> [ FooCmd, _Bar_, BarCmd, ^BarCmd ]
  ASSERT_EQ(4u, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_EQ(1, stack.clean_index());
  ASSERT_EQ("BarCmd", stack.get_undo_text());

  // [ FooCmd, _Bar_, BarCmd, ^BarCmd ] -> [ _Bar_, BarCmd, BarCmd, ^FooCmd ]
  stack.push<FooCmd>();
  ASSERT_EQ(4u, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_EQ(0, stack.clean_index());
  ASSERT_EQ("FooCmd", stack.get_undo_text());

  // [ _Bar_, BarCmd, BarCmd, ^FooCmd ] -> [ BarCmd, BarCmd, FooCmd, ^BarCmd ]
  stack.push<BarCmd>();
  ASSERT_EQ(4u, stack.size());
  ASSERT_EQ(3, stack.index());
  ASSERT_FALSE(stack.clean_index());
  ASSERT_EQ("BarCmd", stack.get_undo_text());
}

TEST(CommandStack, Overflow)
{
  CommandStack stack;
  stack.push<FooCmd>();

  ASSERT_EQ(1u, stack.size());
  ASSERT_EQ(0, stack.index());
  ASSERT_EQ("FooCmd", stack.get_undo_text());

  // The stack should be full after this
  for (usize index = 0u, capacity = stack.capacity(); index < (capacity - 1); ++index) {
    stack.push<BarCmd>();
  }

  ASSERT_EQ(stack.capacity(), stack.size());
  ASSERT_EQ(stack.capacity() - 1, stack.index());
  ASSERT_EQ("BarCmd", stack.get_undo_text());

  stack.push<FooCmd>();  // This should cause the first command to get removed
  ASSERT_EQ(stack.capacity(), stack.size());
}

TEST(CommandStack, SetCapacity)
{
  CommandStack stack;

  stack.set_capacity(5);
  ASSERT_EQ(5u, stack.capacity());

  for (auto index = 0u; index < 5; ++index) {
    stack.push<FooCmd>();
  }
  ASSERT_EQ(5u, stack.size());
  ASSERT_EQ("FooCmd", stack.get_undo_text());

  // [ FooCmd, FooCmd, FooCmd, FooCmd, ^FooCmd ] -> [ FooCmd, FooCmd, FooCmd, FooCmd,
  // ^BarCmd ]
  stack.push<BarCmd>();
  ASSERT_EQ(5u, stack.size());
  ASSERT_EQ(4, stack.index());
  ASSERT_EQ("BarCmd", stack.get_undo_text());

  // [ FooCmd, FooCmd, FooCmd, FooCmd, ^BarCmd ] -> [ FooCmd, FooCmd, ^BarCmd ]
  stack.set_capacity(3);
  ASSERT_EQ(3u, stack.capacity());
  ASSERT_EQ(3u, stack.size());
  ASSERT_EQ(2, stack.index());
  ASSERT_EQ("BarCmd", stack.get_undo_text());

  stack.set_capacity(10);
  ASSERT_EQ(10u, stack.capacity());
  ASSERT_EQ(3u, stack.size());
  ASSERT_EQ(2, stack.index());
  ASSERT_EQ("BarCmd", stack.get_undo_text());
}

}  // namespace tactile::test
