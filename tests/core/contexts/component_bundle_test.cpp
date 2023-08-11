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

#include "tactile/core/contexts/component_bundle.hpp"

#include <gtest/gtest.h>

#include "tactile/core/common/error.hpp"

using namespace tactile;

TEST(ComponentBundle, Attach)
{
  const ComponentInstance component {42, PropertyBundle {}};

  ComponentBundle components;
  EXPECT_FALSE(components.has(component.type_id));

  components.attach(component);
  EXPECT_THROW(components.attach(component), Error);

  EXPECT_EQ(components.size(), 1);
  EXPECT_TRUE(components.has(component.type_id));
}

TEST(ComponentBundle, Detach)
{
  const ComponentInstance component {7, PropertyBundle {}};

  ComponentBundle components;
  components.attach(component);

  auto detached_component = components.detach(component.type_id);
  EXPECT_THROW(components.detach(component.type_id), Error);

  EXPECT_FALSE(components.has(component.type_id));
  EXPECT_EQ(components.size(), 0);
  EXPECT_EQ(detached_component.type_id, component.type_id);
}

TEST(ComponentBundle, Get)
{
  const ComponentInstance component {100, PropertyBundle {}};

  ComponentBundle components;
  components.attach(component);

  EXPECT_NO_THROW((void) components.get(component.type_id));
  EXPECT_THROW((void) components.get(component.type_id + 1), Error);
}
