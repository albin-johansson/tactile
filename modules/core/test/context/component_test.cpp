#include "tactile/core/context/component.hpp"

#include <iterator>  // distance

#include <gtest/gtest.h>

using namespace tactile;

using tactile::int_literals::operator""_uz;
using tactile::int_literals::operator""_z;

/// \tests tactile::Component::Component
TEST(Component, SimpleConstruction)
{
  const ComponentID id {42};
  const Component component {id};

  EXPECT_EQ(component.get_id(), id);
  EXPECT_EQ(component.attribute_count(), 0_uz);
  EXPECT_EQ(std::distance(component.begin(), component.end()), 0_z);
}

/// \tests tactile::Component::Component
TEST(Component, WithInitialAttributes)
{
  const Component::AttributeMap attributes = {
    {"A", 832},
    {"B", "foo"},
  };

  const ComponentID id {99};
  const Component component {id, attributes};

  EXPECT_EQ(component.get_id(), id);
  EXPECT_EQ(component.attribute_count(), 2_uz);
  EXPECT_EQ(std::distance(component.begin(), component.end()), 2_z);
  EXPECT_TRUE(component.has_attribute("A"));
  EXPECT_TRUE(component.has_attribute("B"));
}

/// \tests tactile::Component::add_attribute
/// \tests tactile::Component::has_attribute
TEST(Component, AddAttribute)
{
  Component component {ComponentID {1}};

  EXPECT_EQ(component.attribute_count(), 0_uz);
  EXPECT_FALSE(component.has_attribute("A"));
  EXPECT_FALSE(component.has_attribute("B"));

  component.add_attribute("A");
  EXPECT_EQ(component.attribute_count(), 1_uz);
  EXPECT_TRUE(component.has_attribute("A"));
  EXPECT_FALSE(component.has_attribute("B"));
  EXPECT_FALSE(component.has_attribute("C"));

  component.add_attribute("B");
  EXPECT_EQ(component.attribute_count(), 2_uz);
  EXPECT_TRUE(component.has_attribute("A"));
  EXPECT_TRUE(component.has_attribute("B"));
  EXPECT_FALSE(component.has_attribute("C"));

  component.add_attribute("C");
  EXPECT_EQ(component.attribute_count(), 3_uz);
  EXPECT_TRUE(component.has_attribute("A"));
  EXPECT_TRUE(component.has_attribute("B"));
  EXPECT_TRUE(component.has_attribute("C"));

  EXPECT_EQ(std::distance(component.begin(), component.end()), 3_z);
}

/// \tests tactile::Component::add_attribute
TEST(Component, AddAttributeWithSameName)
{
  Component component {ComponentID {99}};

  auto& a1 = component.add_attribute("A1");
  EXPECT_EQ(a1.get_type(), AttributeType::kStr);
  EXPECT_EQ(component.attribute_count(), 1_uz);
  EXPECT_TRUE(component.has_attribute("A1"));

  auto& a2 = component.add_attribute("A1");
  EXPECT_EQ(&a1, &a2);
  EXPECT_EQ(component.attribute_count(), 1_uz);
  EXPECT_TRUE(component.has_attribute("A1"));
}

/// \tests tactile::Component::remove_attribute
TEST(Component, RemoveAttribute)
{
  const String attr_name {"foo"};
  Component component {ComponentID {12}};

  component.add_attribute(attr_name).set("bar");
  EXPECT_TRUE(component.has_attribute(attr_name));

  const auto attribute = component.remove_attribute(attr_name);
  EXPECT_FALSE(component.has_attribute(attr_name));
  EXPECT_TRUE(attribute.has_value());
  EXPECT_EQ(attribute, "bar");

  EXPECT_FALSE(component.remove_attribute(attr_name).has_value());
}

/// \tests tactile::Component::rename_attribute
TEST(Component, RenameAttribute)
{
  Component component {ComponentID {12}};

  const String old_name {"1"};
  const String new_name {"2"};
  const Attribute value {42};

  EXPECT_FALSE(component.rename_attribute(old_name, new_name));

  EXPECT_FALSE(component.has_attribute(old_name));
  EXPECT_FALSE(component.has_attribute(new_name));
  EXPECT_EQ(component.attribute_count(), 0_uz);

  component.add_attribute(old_name) = value;

  EXPECT_TRUE(component.has_attribute(old_name));
  EXPECT_FALSE(component.has_attribute(new_name));
  EXPECT_EQ(component.attribute_count(), 1_uz);

  EXPECT_FALSE(component.rename_attribute(old_name, old_name));
  EXPECT_TRUE(component.rename_attribute(old_name, new_name));

  EXPECT_FALSE(component.has_attribute(old_name));
  ASSERT_TRUE(component.has_attribute(new_name));
  EXPECT_EQ(component.attribute_count(), 1_uz);
  EXPECT_EQ(component.get_attribute(new_name), value);
}

/// \tests tactile::Component::get_attribute
TEST(Component, GetAttribute)
{
  Component component {ComponentID {12}};
  component.add_attribute("hello").set("world");

  const auto& const_component = component;

  EXPECT_THROW((void) component.get_attribute("world"), RuntimeError);
  EXPECT_THROW((void) const_component.get_attribute("world"), RuntimeError);

  EXPECT_EQ(component.get_attribute("hello"), "world");
  EXPECT_EQ(const_component.get_attribute("hello"), "world");
}
