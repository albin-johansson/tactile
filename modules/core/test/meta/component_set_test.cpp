// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/component_set.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/container/set.hpp"
#include "tactile/foundation/container/string.hpp"
#include "tactile/foundation/debug/exception.hpp"

using namespace tactile;

/// \tests tactile::ComponentSet::define_component
TEST(ComponentSet, DefineComponent)
{
  ComponentSet component_set {};
  EXPECT_EQ(component_set.component_count(), 0);

  const auto component_uuid = component_set.define_component("test");

  ASSERT_TRUE(component_uuid.has_value());
  EXPECT_TRUE(component_set.has_component(*component_uuid));
  EXPECT_EQ(component_set.component_count(), 1);
}

/// \tests tactile::ComponentSet::define_component
TEST(ComponentSet, DefineComponentWithDuplicatedName)
{
  ComponentSet component_set {};

  EXPECT_TRUE(component_set.define_component("foo").has_value());
  EXPECT_FALSE(component_set.define_component("foo").has_value());

  EXPECT_EQ(component_set.component_count(), 1);
}

/// \tests tactile::ComponentSet::remove_component
TEST(ComponentSet, RemoveComponent)
{
  ComponentSet component_set {};

  const auto c1 = component_set.define_component("1").value();
  const auto c2 = component_set.define_component("2").value();
  const auto c3 = component_set.define_component("3").value();
  ASSERT_EQ(component_set.component_count(), 3);

  EXPECT_TRUE(component_set.remove_component(c2).has_value());

  EXPECT_EQ(component_set.component_count(), 2);
  EXPECT_TRUE(component_set.has_component(c1));
  EXPECT_FALSE(component_set.has_component(c2));
  EXPECT_TRUE(component_set.has_component(c3));

  EXPECT_TRUE(component_set.remove_component(c3).has_value());

  EXPECT_EQ(component_set.component_count(), 1);
  EXPECT_TRUE(component_set.has_component(c1));
  EXPECT_FALSE(component_set.has_component(c2));
  EXPECT_FALSE(component_set.has_component(c3));

  EXPECT_TRUE(component_set.remove_component(c1).has_value());

  EXPECT_EQ(component_set.component_count(), 0);
  EXPECT_FALSE(component_set.has_component(c1));
  EXPECT_FALSE(component_set.has_component(c2));
  EXPECT_FALSE(component_set.has_component(c3));
}

/// \tests tactile::ComponentSet::remove_component
TEST(ComponentSet, RemoveComponentWithInvalidIdentifier)
{
  ComponentSet component_set {};

  const auto bad_component_uuid = UUID::generate();
  EXPECT_FALSE(component_set.remove_component(bad_component_uuid).has_value());
}

/// \tests tactile::ComponentSet::get_component_with_name
TEST(ComponentSet, GetComponentWithName)
{
  ComponentSet component_set {};

  const String component_name {"foobar"};
  const String bad_component_name {"barfoo"};

  const auto component_uuid = component_set.define_component(component_name);

  EXPECT_EQ(component_set.get_component_with_name(component_name), component_uuid);
  EXPECT_EQ(component_set.get_component_with_name(bad_component_name), kNothing);
}

/// \tests tactile::ComponentSet::get_component
TEST(ComponentSet, GetComponent)
{
  ComponentSet component_set {};

  const auto component_uuid = component_set.define_component("123456").value();
  const auto bad_component_uuid = UUID::generate();

  const auto& const_component_set = component_set;

  EXPECT_NO_THROW((void) component_set.get_component(component_uuid));
  EXPECT_NO_THROW((void) const_component_set.get_component(component_uuid));

  EXPECT_THROW((void) component_set.get_component(bad_component_uuid), Exception);
  EXPECT_THROW((void) const_component_set.get_component(bad_component_uuid), Exception);
}

/// \tests tactile::ComponentSet::each
TEST(ComponentSet, Each)
{
  ComponentSet component_set {};

  const auto a [[maybe_unused]] = component_set.define_component("A").value();
  const auto b [[maybe_unused]] = component_set.define_component("B").value();
  const auto c [[maybe_unused]] = component_set.define_component("C").value();

  Set<String> visited_component_names {};
  component_set.each([&](const ComponentDefinition& definition) {
    visited_component_names.insert(definition.name);
  });

  EXPECT_TRUE(visited_component_names.contains("A"));
  EXPECT_TRUE(visited_component_names.contains("B"));
  EXPECT_TRUE(visited_component_names.contains("C"));
}
