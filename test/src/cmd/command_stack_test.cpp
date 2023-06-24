/*
 * This source file is a part of the Tactile map editor.
 *
 * Copyright (C) 2023 Albin Johansson
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

#include "cmd/command_stack.hpp"

#include <doctest/doctest.h>

using namespace tactile;

namespace {

struct FooCmd final : Command {
  void undo() override {}
  void redo() override {}
  [[nodiscard]] auto get_name() const -> String override { return "FooCmd"; }
};

struct BarCmd final : Command {
  void undo() override {}
  void redo() override {}
  [[nodiscard]] auto get_name() const -> String override { return "BarCmd"; }
};

}  // namespace

TEST_SUITE("CommandStack")
{
  TEST_CASE("defaults")
  {
    const CommandStack stack;
    CHECK(stack.size() == 0u);
    CHECK(stack.capacity() == 64u);
    CHECK(!stack.can_undo());
    CHECK(!stack.can_redo());
    CHECK(stack.is_clean());
  }

  TEST_CASE("usage")
  {
    CommandStack stack;

    // ^[ ] -> [ ^FooCmd ]
    stack.push<FooCmd>();
    CHECK(stack.size() == 1u);
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());
    CHECK(stack.get_undo_text() == "FooCmd");

    // [ ^FooCmd ] -> [ FooCmd, ^BarCmd ]
    stack.push<BarCmd>();
    CHECK(stack.size() == 2u);
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());
    CHECK(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.undo();
    CHECK(stack.size() == 2u);
    CHECK(stack.can_undo());
    CHECK(stack.can_redo());
    CHECK(stack.get_undo_text() == "FooCmd");
    CHECK(stack.get_redo_text() == "BarCmd");

    // [ ^FooCmd, BarCmd ] -> ^[ FooCmd, BarCmd ]
    stack.undo();
    CHECK(stack.size() == 2u);
    CHECK(!stack.can_undo());
    CHECK(stack.can_redo());
    CHECK(stack.get_redo_text() == "FooCmd");

    // ^[ FooCmd, BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.redo();
    CHECK(stack.size() == 2u);
    CHECK(stack.can_undo());
    CHECK(stack.can_redo());
    CHECK(stack.get_undo_text() == "FooCmd");
    CHECK(stack.get_redo_text() == "BarCmd");

    // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^BarCmd ]
    stack.redo();
    CHECK(stack.size() == 2u);
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());
    CHECK(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.undo();
    CHECK(stack.size() == 2u);
    CHECK(stack.can_undo());
    CHECK(stack.can_redo());
    CHECK(stack.get_undo_text() == "FooCmd");
    CHECK(stack.get_redo_text() == "BarCmd");

    // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^FooCmd ]
    stack.push<FooCmd>();
    CHECK(stack.size() == 2u);
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());
    CHECK(stack.get_undo_text() == "FooCmd");

    // [ FooCmd, ^FooCmd ] -> ^[ FooCmd, FooCmd ]
    stack.undo();
    stack.undo();
    CHECK(stack.size() == 2u);
    CHECK(stack.get_redo_text() == "FooCmd");

    // ^[ FooCmd, FooCmd ] -> [ ^BarCmd ]
    stack.push<BarCmd>();
    CHECK(stack.size() == 1u);
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());
    CHECK(stack.get_undo_text() == "BarCmd");
  }

  TEST_CASE("clean")
  {
    CommandStack stack;

    CHECK(stack.is_clean());
    CHECK(!stack.can_undo());
    CHECK(!stack.can_redo());

    stack.mark_as_clean();
    stack.push<FooCmd>();
    CHECK(!stack.is_clean());

    stack.undo();
    CHECK(stack.is_clean());

    // ^[ ] -> [ ^FooCmd ]
    stack.clear();
    stack.reset_clean();
    stack.push<FooCmd>();
    CHECK(!stack.is_clean());
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());

    // [ ^FooCmd ] -> [ FooCmd, ^BarCmd ]
    stack.push<BarCmd>();
    CHECK(!stack.is_clean());
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());

    stack.mark_as_clean();
    CHECK(stack.is_clean());
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());

    // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.undo();
    CHECK(!stack.is_clean());
    CHECK(stack.can_undo());
    CHECK(stack.can_redo());

    // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^BarCmd ]
    stack.redo();
    CHECK(stack.is_clean());
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());

    // Here we test a special case when the clean state becomes invalidated
    // [ FooCmd, ^BarCmd ] -Undo-> [ ^FooCmd, BarCmd ] -Push-> [ FooCmd, ^FooCmd ]
    stack.undo();
    stack.push<FooCmd>();
    CHECK(!stack.is_clean());
    CHECK(stack.can_undo());
    CHECK(!stack.can_redo());

    stack.mark_as_clean();
    CHECK(stack.is_clean());

    stack.reset_clean();
    CHECK(!stack.is_clean());
  }

  TEST_CASE("overflow with clean index")
  {
    CommandStack stack;

    stack.set_capacity(4);
    CHECK(stack.capacity() == 4u);

    stack.push<FooCmd>();

    stack.push<BarCmd>();
    stack.mark_as_clean();

    stack.push<BarCmd>();
    stack.push<BarCmd>();

    //  ^[ ] -> [ FooCmd, _Bar_, BarCmd, ^BarCmd ]
    CHECK(stack.size() == 4u);
    CHECK(stack.index() == 3);
    CHECK(stack.clean_index() == 1);
    CHECK(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, _Bar_, BarCmd, ^BarCmd ] -> [ _Bar_, BarCmd, BarCmd, ^FooCmd ]
    stack.push<FooCmd>();
    CHECK(stack.size() == 4u);
    CHECK(stack.index() == 3);
    CHECK(stack.clean_index() == 0);
    CHECK(stack.get_undo_text() == "FooCmd");

    // [ _Bar_, BarCmd, BarCmd, ^FooCmd ] -> [ BarCmd, BarCmd, FooCmd, ^BarCmd ]
    stack.push<BarCmd>();
    CHECK(stack.size() == 4u);
    CHECK(stack.index() == 3);
    CHECK(!stack.clean_index());
    CHECK(stack.get_undo_text() == "BarCmd");
  }

  TEST_CASE("overflow")
  {
    CommandStack stack;
    stack.push<FooCmd>();

    CHECK(stack.size() == 1u);
    CHECK(stack.index() == 0);
    CHECK(stack.get_undo_text() == "FooCmd");

    // The stack should be full after this
    for (usize index = 0u, capacity = stack.capacity(); index < (capacity - 1); ++index) {
      stack.push<BarCmd>();
    }

    CHECK(stack.capacity() == stack.size());
    CHECK(stack.capacity() - 1 == stack.index());
    CHECK(stack.get_undo_text() == "BarCmd");

    stack.push<FooCmd>();  // This should cause the first command to get removed
    CHECK(stack.capacity() == stack.size());
  }

  TEST_CASE("set_capacity")
  {
    CommandStack stack;

    stack.set_capacity(5);
    CHECK(stack.capacity() == 5u);

    for (auto index = 0u; index < 5; ++index) {
      stack.push<FooCmd>();
    }
    CHECK(stack.size() == 5u);
    CHECK(stack.get_undo_text() == "FooCmd");

    // [ FooCmd, FooCmd, FooCmd, FooCmd, ^FooCmd ] -> [ FooCmd, FooCmd, FooCmd, FooCmd,
    // ^BarCmd ]
    stack.push<BarCmd>();
    CHECK(stack.size() == 5u);
    CHECK(stack.index() == 4);
    CHECK(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, FooCmd, FooCmd, FooCmd, ^BarCmd ] -> [ FooCmd, FooCmd, ^BarCmd ]
    stack.set_capacity(3);
    CHECK(stack.capacity() == 3u);
    CHECK(stack.size() == 3u);
    CHECK(stack.index() == 2);
    CHECK(stack.get_undo_text() == "BarCmd");

    stack.set_capacity(10);
    CHECK(stack.capacity() == 10u);
    CHECK(stack.size() == 3u);
    CHECK(stack.index() == 2);
    CHECK(stack.get_undo_text() == "BarCmd");
  }
}
