#include "core/systems/property_system.hpp"

#include <string>  // string_literals

#include <tactile_stdlib.hpp>

#include <gtest/gtest.h>

#include "core/systems/registry_system.hpp"

using namespace tactile;

class PropertySystemTest : public testing::Test {
 protected:
  void SetUp() override
  {
    sys::ResetNextContextId();
    mRegistry = sys::make_document_registry();
  }

  entt::registry mRegistry;
};

TEST_F(PropertySystemTest, AddPropertyContext)
{
  const auto a = mRegistry.create();
  const auto b = mRegistry.create();

  const auto& ac = sys::AddPropertyContext(mRegistry, a);
  ASSERT_GT(ac.id, 0);
  ASSERT_TRUE(ac.name.empty());
  ASSERT_TRUE(ac.properties.empty());

  const auto& bc = sys::AddPropertyContext(mRegistry, b);
  ASSERT_GT(bc.id, ac.id);
  ASSERT_TRUE(bc.name.empty());
  ASSERT_TRUE(bc.properties.empty());
}

TEST_F(PropertySystemTest, CopyPropertyContext)
{
  const auto entity = mRegistry.create();
  auto& context = sys::AddPropertyContext(mRegistry, entity);
  context.name = "Foo";

  using namespace std::string_literals;
  sys::AddProperty(mRegistry, context, "A", 123);
  sys::AddProperty(mRegistry, context, "B", "abc"s);

  const auto snapshot = sys::CopyPropertyContext(mRegistry, entity);
  ASSERT_EQ(context.id, snapshot.id);
  ASSERT_EQ(context.name, snapshot.name);

  ASSERT_EQ(2u, snapshot.properties.size());
  ASSERT_EQ(123, snapshot.properties.at("A").as_int());
  ASSERT_EQ("abc", snapshot.properties.at("B").as_string());
}

TEST_F(PropertySystemTest, RestorePropertyContext)
{
  const auto oldEntity = mRegistry.create();
  const auto id = sys::GetNextContextId();

  {
    auto& context = sys::AddPropertyContext(mRegistry, oldEntity);
    context.name = "ABC";

    ASSERT_EQ(id, context.id);

    sys::AddProperty(mRegistry, context, "A", 12);
    sys::AddProperty(mRegistry, context, "B", 3.14f);

    ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "A"));
    ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "B"));
  }

  const auto snapshot = sys::CopyPropertyContext(mRegistry, oldEntity);

  {
    const auto newEntity = mRegistry.create();
    sys::RestorePropertyContext(mRegistry, newEntity, snapshot);

    const auto& context = mRegistry.get<attribute_context>(newEntity);
    ASSERT_EQ(id, context.id);
    ASSERT_EQ("ABC", context.name);

    ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "A"));
    ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "B"));

    ASSERT_EQ(12, sys::GetProperty(mRegistry, context, "A").value.as_int());
    ASSERT_EQ(3.14f, sys::GetProperty(mRegistry, context, "B").value.as_float());
  }
}

TEST_F(PropertySystemTest, AddPropertyWithType)
{
  const auto entity = mRegistry.create();
  auto& context = sys::AddPropertyContext(mRegistry, entity);

  sys::AddProperty(mRegistry, context, "str", PropertyType::String);
  ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "str"));
  ASSERT_EQ("", sys::GetProperty(mRegistry, context, "str").value.as_string());
}

TEST_F(PropertySystemTest, AddPropertyWithValue)
{
  const auto entity = mRegistry.create();
  auto& context = sys::AddPropertyContext(mRegistry, entity);

  sys::AddProperty(mRegistry, context, "int", 123);
  ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "int"));
  ASSERT_EQ(123, sys::GetProperty(mRegistry, context, "int").value.as_int());
}

TEST_F(PropertySystemTest, RemoveProperty)
{
  const auto entity = mRegistry.create();
  auto& context = sys::AddPropertyContext(mRegistry, entity);

  sys::AddProperty(mRegistry, context, "X", PropertyType::Integer);
  ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "X"));
  ASSERT_EQ(1u, context.properties.size());

  sys::RemoveProperty(mRegistry, context, "X");
  ASSERT_FALSE(sys::HasPropertyWithName(mRegistry, context, "X"));
  ASSERT_EQ(0u, context.properties.size());
}

TEST_F(PropertySystemTest, RenameProperty)
{
  auto& context = mRegistry.ctx<attribute_context>();

  sys::AddProperty(mRegistry, context, "abc", PropertyType::Color);
  ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "abc"));

  sys::RenameProperty(mRegistry, context, "abc", "def");
  ASSERT_FALSE(sys::HasPropertyWithName(mRegistry, context, "abc"));
  ASSERT_TRUE(sys::HasPropertyWithName(mRegistry, context, "def"));
}

TEST_F(PropertySystemTest, UpdateProperty)
{
  auto& context = mRegistry.ctx<attribute_context>();

  sys::AddProperty(mRegistry, context, "xyz", 45.3f);
  ASSERT_EQ(45.3f, sys::GetProperty(mRegistry, context, "xyz").value.as_float());

  sys::UpdateProperty(mRegistry, context, "xyz", 123.5f);
  ASSERT_EQ(123.5f, sys::GetProperty(mRegistry, context, "xyz").value.as_float());
}

TEST_F(PropertySystemTest, ChangePropertyType)
{
  auto& context = mRegistry.ctx<attribute_context>();

  sys::AddProperty(mRegistry, context, "foo", 123);
  ASSERT_TRUE(sys::GetProperty(mRegistry, context, "foo").value.is_int());

  sys::ChangePropertyType(mRegistry, context, "foo", PropertyType::String);
  ASSERT_TRUE(sys::GetProperty(mRegistry, context, "foo").value.is_string());
  ASSERT_EQ("", sys::GetProperty(mRegistry, context, "foo").value.as_string());

  sys::ChangePropertyType(mRegistry, context, "foo", PropertyType::Integer);
  ASSERT_TRUE(sys::GetProperty(mRegistry, context, "foo").value.is_int());
  ASSERT_EQ(0, sys::GetProperty(mRegistry, context, "foo").value.as_int());
}

TEST_F(PropertySystemTest, GetContext)
{
  ASSERT_NO_THROW(sys::GetContext(mRegistry, mRegistry.ctx<attribute_context>().id));
  ASSERT_THROW(sys::GetContext(mRegistry, sys::GetNextContextId()), TactileError);
}

TEST_F(PropertySystemTest, GetProperty)
{
  auto& context = mRegistry.ctx<attribute_context>();
  ASSERT_THROW(sys::GetProperty(mRegistry, context, "foo"), TactileError);

  sys::AddProperty(mRegistry, context, "foo", 42);
  const auto& property = sys::GetProperty(mRegistry, context, "foo");

  ASSERT_EQ("foo", property.name);
  ASSERT_EQ(42, property.value.as_int());
}