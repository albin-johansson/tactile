#include "core/systems/property_system.hpp"

#include <string>  // string_literals

#include <gtest/gtest.h>

#include "core/systems/context_system.hpp"
#include "core/systems/registry_system.hpp"
#include "misc/throw.hpp"

using namespace tactile;

class PropertySystemTest : public testing::Test
{
 protected:
  void SetUp() override
  {
    sys::reset_next_context_id();
    mRegistry = sys::make_document_registry();
  }

  entt::registry mRegistry;
};

TEST_F(PropertySystemTest, AddPropertyWithType)
{
  const auto entity = mRegistry.create();
  auto& context = sys::add_attribute_context(mRegistry, entity);

  sys::add_property(mRegistry, context, "str", AttributeType::string);
  ASSERT_TRUE(sys::has_property_with_name(mRegistry, context, "str"));
  ASSERT_EQ("", sys::get_property(mRegistry, context, "str").value.as_string());
}

TEST_F(PropertySystemTest, AddPropertyWithValue)
{
  const auto entity = mRegistry.create();
  auto& context = sys::add_attribute_context(mRegistry, entity);

  sys::add_property(mRegistry, context, "int", 123);
  ASSERT_TRUE(sys::has_property_with_name(mRegistry, context, "int"));
  ASSERT_EQ(123, sys::get_property(mRegistry, context, "int").value.as_int());
}

TEST_F(PropertySystemTest, RemoveProperty)
{
  const auto entity = mRegistry.create();
  auto& context = sys::add_attribute_context(mRegistry, entity);

  sys::add_property(mRegistry, context, "X", AttributeType::integer);
  ASSERT_TRUE(sys::has_property_with_name(mRegistry, context, "X"));
  ASSERT_EQ(1u, context.properties.size());

  sys::remove_property(mRegistry, context, "X");
  ASSERT_FALSE(sys::has_property_with_name(mRegistry, context, "X"));
  ASSERT_EQ(0u, context.properties.size());
}

TEST_F(PropertySystemTest, RenameProperty)
{
  auto& context = mRegistry.ctx<comp::AttributeContext>();

  sys::add_property(mRegistry, context, "abc", AttributeType::color);
  ASSERT_TRUE(sys::has_property_with_name(mRegistry, context, "abc"));

  sys::rename_property(mRegistry, context, "abc", "def");
  ASSERT_FALSE(sys::has_property_with_name(mRegistry, context, "abc"));
  ASSERT_TRUE(sys::has_property_with_name(mRegistry, context, "def"));
}

TEST_F(PropertySystemTest, UpdateProperty)
{
  auto& context = mRegistry.ctx<comp::AttributeContext>();

  sys::add_property(mRegistry, context, "xyz", 45.3f);
  ASSERT_EQ(45.3f, sys::get_property(mRegistry, context, "xyz").value.as_float());

  sys::update_property(mRegistry, context, "xyz", 123.5f);
  ASSERT_EQ(123.5f, sys::get_property(mRegistry, context, "xyz").value.as_float());
}

TEST_F(PropertySystemTest, ChangePropertyType)
{
  auto& context = mRegistry.ctx<comp::AttributeContext>();

  sys::add_property(mRegistry, context, "foo", 123);
  ASSERT_TRUE(sys::get_property(mRegistry, context, "foo").value.is_int());

  sys::change_property_type(mRegistry, context, "foo", AttributeType::string);
  ASSERT_TRUE(sys::get_property(mRegistry, context, "foo").value.is_string());
  ASSERT_EQ("", sys::get_property(mRegistry, context, "foo").value.as_string());

  sys::change_property_type(mRegistry, context, "foo", AttributeType::integer);
  ASSERT_TRUE(sys::get_property(mRegistry, context, "foo").value.is_int());
  ASSERT_EQ(0, sys::get_property(mRegistry, context, "foo").value.as_int());
}

TEST_F(PropertySystemTest, GetContext)
{
  ASSERT_NO_THROW(
      sys::get_context(mRegistry, mRegistry.ctx<comp::AttributeContext>().id));
  ASSERT_THROW(sys::get_context(mRegistry, sys::next_context_id()), tactile_error);
}

TEST_F(PropertySystemTest, GetProperty)
{
  auto& context = mRegistry.ctx<comp::AttributeContext>();
  ASSERT_THROW(sys::get_property(mRegistry, context, "foo"), tactile_error);

  sys::add_property(mRegistry, context, "foo", 42);
  const auto& property = sys::get_property(mRegistry, context, "foo");

  ASSERT_EQ("foo", property.name);
  ASSERT_EQ(42, property.value.as_int());
}