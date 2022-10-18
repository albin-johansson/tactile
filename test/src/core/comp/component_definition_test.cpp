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

#include "core/comp/component_definition.hpp"

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace eastl::string_literals;

namespace tactile::test {

TEST(ComponentDefinition, SetName)
{
  ComponentDefinition definition;
  ASSERT_TRUE(definition.name().empty());

  definition.set_name("Qwerty");
  ASSERT_EQ("Qwerty", definition.name());
}

TEST(ComponentDefinition, Instantiate)
{
  ComponentDefinition definition;
  definition.add("i", 42);
  definition.add("f", 1.8f);
  definition.add("s", "foo"s);
  definition.add("c", cen::colors::gold);

  const auto instance = definition.instantiate();
  ASSERT_EQ(definition.uuid(), instance.definition_id());
  ASSERT_EQ(definition.size(), instance.size());

  for (const auto& [key, value]: instance) {
    ASSERT_EQ(definition.at(key), value);
  }
}

}  // namespace tactile::test
