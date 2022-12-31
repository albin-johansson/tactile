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

#include "common/util/numeric.hpp"

#include <limits>  // numeric_limits

#include <gtest/gtest.h>

#include "core/vocabulary.hpp"

namespace tactile::test {
namespace {

constexpr auto u32_max = std::numeric_limits<uint32>::max();

}  // namespace

TEST(Numeric, Udiff)
{
  ASSERT_EQ(1u, udiff(u32_max, u32_max - 1));
  ASSERT_EQ(1u, udiff(u32_max - 1, u32_max));

  ASSERT_EQ(u32_max, udiff(0u, u32_max));

  ASSERT_EQ(46u, udiff(28u, 74u));
}

}  // namespace tactile::test