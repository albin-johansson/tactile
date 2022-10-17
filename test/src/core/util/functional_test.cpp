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

#include "core/util/functional.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(InvokeN, ZeroInvocations)
{
  bool called = false;
  invoke_n(0, [&] { called = true; });
  ASSERT_FALSE(called);
}

TEST(InvokeN, OneInvocation)
{
  int calls = 0;
  invoke_n(1, [&] { ++calls; });
  ASSERT_EQ(1, calls);
}

TEST(InvokeN, SeveralInvocations)
{
  int calls = 0;
  invoke_n(42, [&] { ++calls; });
  ASSERT_EQ(42, calls);
}

}  // namespace tactile::test
