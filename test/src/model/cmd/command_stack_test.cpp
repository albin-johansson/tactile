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

#include <doctest/doctest.h>

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

TEST_SUITE("CommandStack")
{
  TEST_CASE("defaults")
  {
    const CommandStack stack;
    REQUIRE(stack.size() == 0u);
    REQUIRE(io::get_preferences().command_capacity == stack.capacity());
    REQUIRE(!stack.can_undo());
    REQUIRE(!stack.can_redo());
    REQUIRE(stack.is_clean());
  }

  TEST_CASE("usage")
  {
    CommandStack stack;

    // ^[ ] -> [ ^FooCmd ]
    stack.push<FooCmd>();
    REQUIRE(stack.size() == 1u);
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());
    REQUIRE(stack.get_undo_text() == "FooCmd");

    // [ ^FooCmd ] -> [ FooCmd, ^BarCmd ]
    stack.push<BarCmd>();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());
    REQUIRE(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.undo();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.can_undo());
    REQUIRE(stack.can_redo());
    REQUIRE(stack.get_undo_text() == "FooCmd");
    REQUIRE(stack.get_redo_text() == "BarCmd");

    // [ ^FooCmd, BarCmd ] -> ^[ FooCmd, BarCmd ]
    stack.undo();
    REQUIRE(stack.size() == 2u);
    REQUIRE(!stack.can_undo());
    REQUIRE(stack.can_redo());
    REQUIRE(stack.get_redo_text() == "FooCmd");

    // ^[ FooCmd, BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.redo();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.can_undo());
    REQUIRE(stack.can_redo());
    REQUIRE(stack.get_undo_text() == "FooCmd");
    REQUIRE(stack.get_redo_text() == "BarCmd");

    // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^BarCmd ]
    stack.redo();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());
    REQUIRE(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.undo();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.can_undo());
    REQUIRE(stack.can_redo());
    REQUIRE(stack.get_undo_text() == "FooCmd");
    REQUIRE(stack.get_redo_text() == "BarCmd");

    // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^FooCmd ]
    stack.push<FooCmd>();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());
    REQUIRE(stack.get_undo_text() == "FooCmd");

    // [ FooCmd, ^FooCmd ] -> ^[ FooCmd, FooCmd ]
    stack.undo();
    stack.undo();
    REQUIRE(stack.size() == 2u);
    REQUIRE(stack.get_redo_text() == "FooCmd");

    // ^[ FooCmd, FooCmd ] -> [ ^BarCmd ]
    stack.push<BarCmd>();
    REQUIRE(stack.size() == 1u);
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());
    REQUIRE(stack.get_undo_text() == "BarCmd");
  }

  TEST_CASE("clean")
  {
    CommandStack stack;

    REQUIRE(stack.is_clean());
    REQUIRE(!stack.can_undo());
    REQUIRE(!stack.can_redo());

    stack.mark_as_clean();
    stack.push<FooCmd>();
    REQUIRE(!stack.is_clean());

    stack.undo();
    REQUIRE(stack.is_clean());

    // ^[ ] -> [ ^FooCmd ]
    stack.clear();
    stack.reset_clean();
    stack.push<FooCmd>();
    REQUIRE(!stack.is_clean());
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());

    // [ ^FooCmd ] -> [ FooCmd, ^BarCmd ]
    stack.push<BarCmd>();
    REQUIRE(!stack.is_clean());
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());

    stack.mark_as_clean();
    REQUIRE(stack.is_clean());
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());

    // [ FooCmd, ^BarCmd ] -> [ ^FooCmd, BarCmd ]
    stack.undo();
    REQUIRE(!stack.is_clean());
    REQUIRE(stack.can_undo());
    REQUIRE(stack.can_redo());

    // [ ^FooCmd, BarCmd ] -> [ FooCmd, ^BarCmd ]
    stack.redo();
    REQUIRE(stack.is_clean());
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());

    // Here we test a special case when the clean state becomes invalidated
    // [ FooCmd, ^BarCmd ] -Undo-> [ ^FooCmd, BarCmd ] -Push-> [ FooCmd, ^FooCmd ]
    stack.undo();
    stack.push<FooCmd>();
    REQUIRE(!stack.is_clean());
    REQUIRE(stack.can_undo());
    REQUIRE(!stack.can_redo());

    stack.mark_as_clean();
    REQUIRE(stack.is_clean());

    stack.reset_clean();
    REQUIRE(!stack.is_clean());
  }

  TEST_CASE("overflow with clean index")
  {
    CommandStack stack;

    stack.set_capacity(4);
    REQUIRE(stack.capacity() == 4u);

    stack.push<FooCmd>();

    stack.push<BarCmd>();
    stack.mark_as_clean();

    stack.push<BarCmd>();
    stack.push<BarCmd>();

    //  ^[ ] -> [ FooCmd, _Bar_, BarCmd, ^BarCmd ]
    REQUIRE(stack.size() == 4u);
    REQUIRE(stack.index() == 3);
    REQUIRE(stack.clean_index() == 1);
    REQUIRE(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, _Bar_, BarCmd, ^BarCmd ] -> [ _Bar_, BarCmd, BarCmd, ^FooCmd ]
    stack.push<FooCmd>();
    REQUIRE(stack.size() == 4u);
    REQUIRE(stack.index() == 3);
    REQUIRE(stack.clean_index() == 0);
    REQUIRE(stack.get_undo_text() == "FooCmd");

    // [ _Bar_, BarCmd, BarCmd, ^FooCmd ] -> [ BarCmd, BarCmd, FooCmd, ^BarCmd ]
    stack.push<BarCmd>();
    REQUIRE(stack.size() == 4u);
    REQUIRE(stack.index() == 3);
    REQUIRE(!stack.clean_index());
    REQUIRE(stack.get_undo_text() == "BarCmd");
  }

  TEST_CASE("overflow")
  {
    CommandStack stack;
    stack.push<FooCmd>();

    REQUIRE(stack.size() == 1u);
    REQUIRE(stack.index() == 0);
    REQUIRE(stack.get_undo_text() == "FooCmd");

    // The stack should be full after this
    for (usize index = 0u, capacity = stack.capacity(); index < (capacity - 1); ++index) {
      stack.push<BarCmd>();
    }

    REQUIRE(stack.capacity() == stack.size());
    REQUIRE(stack.capacity() - 1 == stack.index());
    REQUIRE(stack.get_undo_text() == "BarCmd");

    stack.push<FooCmd>();  // This should cause the first command to get removed
    REQUIRE(stack.capacity() == stack.size());
  }

  TEST_CASE("set_capacity")
  {
    CommandStack stack;

    stack.set_capacity(5);
    REQUIRE(stack.capacity() == 5u);

    for (auto index = 0u; index < 5; ++index) {
      stack.push<FooCmd>();
    }
    REQUIRE(stack.size() == 5u);
    REQUIRE(stack.get_undo_text() == "FooCmd");

    // [ FooCmd, FooCmd, FooCmd, FooCmd, ^FooCmd ] -> [ FooCmd, FooCmd, FooCmd, FooCmd,
    // ^BarCmd ]
    stack.push<BarCmd>();
    REQUIRE(stack.size() == 5u);
    REQUIRE(stack.index() == 4);
    REQUIRE(stack.get_undo_text() == "BarCmd");

    // [ FooCmd, FooCmd, FooCmd, FooCmd, ^BarCmd ] -> [ FooCmd, FooCmd, ^BarCmd ]
    stack.set_capacity(3);
    REQUIRE(stack.capacity() == 3u);
    REQUIRE(stack.size() == 3u);
    REQUIRE(stack.index() == 2);
    REQUIRE(stack.get_undo_text() == "BarCmd");

    stack.set_capacity(10);
    REQUIRE(stack.capacity() == 10u);
    REQUIRE(stack.size() == 3u);
    REQUIRE(stack.index() == 2);
    REQUIRE(stack.get_undo_text() == "BarCmd");
  }
}

}  // namespace tactile::test
