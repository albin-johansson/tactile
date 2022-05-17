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

#include "core/utils/actor.hpp"

#include <string>  // string

#include <gtest/gtest.h>

#include "misc/panic.hpp"

using namespace tactile;

namespace {

struct FooComponent final
{
  float x{};
  float y{};
};

struct BarComponent final
{
  std::string str;
};

}  // namespace

TEST(Actor, Constructor)
{
  entt::registry registry;
  const auto entity = registry.create();

  const Actor actor{registry, entity};

  ASSERT_EQ(entity, actor.entity());
  ASSERT_EQ(&registry, &actor.registry());

  ASSERT_THROW(Actor(registry, entt::null), TactileError);
}

TEST(Actor, Add)
{
  entt::registry registry;
  const auto entity = registry.create();

  Actor actor{registry, entity};

  ASSERT_FALSE(actor.has<FooComponent>());
  ASSERT_FALSE(actor.has<BarComponent>());

  auto& foo = actor.add<FooComponent>(84.8f, 20.3f);
  ASSERT_EQ(84.8f, foo.x);
  ASSERT_EQ(20.3f, foo.y);

  ASSERT_TRUE(actor.has<FooComponent>());
  ASSERT_FALSE(actor.has<BarComponent>());

  auto& bar = actor.add<BarComponent>("hello");
  ASSERT_EQ("hello", bar.str);

  ASSERT_TRUE(actor.has<FooComponent>());
  ASSERT_TRUE(actor.has<BarComponent>());
}

TEST(Actor, Get)
{
  entt::registry registry;

  const auto entity = registry.create();
  registry.emplace<FooComponent>(entity, 10, 20);

  Actor actor{registry, entity};
  const auto& ref = actor;

  ASSERT_TRUE(actor.has<FooComponent>());
  ASSERT_FALSE(actor.has<BarComponent>());

  ASSERT_EQ(10, actor.get<FooComponent>().x);
  ASSERT_EQ(10, ref.get<FooComponent>().x);

  ASSERT_EQ(20, actor.get<FooComponent>().y);
  ASSERT_EQ(20, ref.get<FooComponent>().y);

  ASSERT_THROW(actor.get<BarComponent>(), TactileError);
  ASSERT_THROW(ref.get<BarComponent>(), TactileError);
}

TEST(Actor, Find)
{
  entt::registry registry;
  const auto entity = registry.create();

  Actor actor{registry, entity};
  actor.add<FooComponent>();

  const auto& ref = actor;

  ASSERT_TRUE(actor.find<FooComponent>());
  ASSERT_TRUE(ref.find<FooComponent>());

  ASSERT_FALSE(actor.find<BarComponent>());
  ASSERT_FALSE(ref.find<BarComponent>());
}