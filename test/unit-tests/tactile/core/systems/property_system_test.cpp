#include "core/systems/property_system.hpp"

#include <string>  // string_literals

#include <tactile_stdlib.hpp>

#include <gtest/gtest.h>

#include "core/systems/registry_system.hpp"

using namespace Tactile;

class PropertySystemTest : public testing::Test
{
 protected:
  void SetUp() override
  {
    Sys::ResetNextContextId();
    mRegistry = Sys::MakeRegistry();
  }

  entt::registry mRegistry;
};

TEST_F(PropertySystemTest, AddPropertyContext)
{
  const auto a = mRegistry.create();
  const auto b = mRegistry.create();

  const auto& ac = Sys::AddPropertyContext(mRegistry, a);
  ASSERT_GT(ac.id, 0);
  ASSERT_TRUE(ac.name.empty());
  ASSERT_TRUE(ac.properties.empty());

  const auto& bc = Sys::AddPropertyContext(mRegistry, b);
  ASSERT_GT(bc.id, ac.id);
  ASSERT_TRUE(bc.name.empty());
  ASSERT_TRUE(bc.properties.empty());
}

TEST_F(PropertySystemTest, CopyPropertyContext)
{
  const auto entity = mRegistry.create();
  auto& context = Sys::AddPropertyContext(mRegistry, entity);
  context.name = "Foo";

  using namespace std::string_literals;
  Sys::AddProperty(mRegistry, context, "A", 123);
  Sys::AddProperty(mRegistry, context, "B", "abc"s);

  const auto snapshot = Sys::CopyPropertyContext(mRegistry, entity);
  ASSERT_EQ(context.id, snapshot.id);
  ASSERT_EQ(context.name, snapshot.name);

  ASSERT_EQ(2u, snapshot.properties.size());
  ASSERT_EQ(123, snapshot.properties.at("A").AsInt());
  ASSERT_EQ("abc", snapshot.properties.at("B").AsString());
}

TEST_F(PropertySystemTest, RestorePropertyContext)
{
  const auto oldEntity = mRegistry.create();
  const auto id = Sys::GetNextContextId();

  {
    auto& context = Sys::AddPropertyContext(mRegistry, oldEntity);
    context.name = "ABC";

    ASSERT_EQ(id, context.id);

    Sys::AddProperty(mRegistry, context, "A", 12);
    Sys::AddProperty(mRegistry, context, "B", 3.14f);

    ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "A"));
    ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "B"));
  }

  const auto snapshot = Sys::CopyPropertyContext(mRegistry, oldEntity);

  {
    const auto newEntity = mRegistry.create();
    Sys::RestorePropertyContext(mRegistry, newEntity, snapshot);

    const auto& context = mRegistry.get<PropertyContext>(newEntity);
    ASSERT_EQ(id, context.id);
    ASSERT_EQ("ABC", context.name);

    ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "A"));
    ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "B"));

    ASSERT_EQ(12, Sys::GetProperty(mRegistry, context, "A").value.AsInt());
    ASSERT_EQ(3.14f, Sys::GetProperty(mRegistry, context, "B").value.AsFloat());
  }
}

TEST_F(PropertySystemTest, AddPropertyWithType)
{
  const auto entity = mRegistry.create();
  auto& context = Sys::AddPropertyContext(mRegistry, entity);

  Sys::AddProperty(mRegistry, context, "str", PropertyType::String);
  ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "str"));
  ASSERT_EQ("", Sys::GetProperty(mRegistry, context, "str").value.AsString());
}

TEST_F(PropertySystemTest, AddPropertyWithValue)
{
  const auto entity = mRegistry.create();
  auto& context = Sys::AddPropertyContext(mRegistry, entity);

  Sys::AddProperty(mRegistry, context, "int", 123);
  ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "int"));
  ASSERT_EQ(123, Sys::GetProperty(mRegistry, context, "int").value.AsInt());
}

TEST_F(PropertySystemTest, RemoveProperty)
{
  const auto entity = mRegistry.create();
  auto& context = Sys::AddPropertyContext(mRegistry, entity);

  Sys::AddProperty(mRegistry, context, "X", PropertyType::Integer);
  ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "X"));
  ASSERT_EQ(1u, context.properties.size());

  Sys::RemoveProperty(mRegistry, context, "X");
  ASSERT_FALSE(Sys::HasPropertyWithName(mRegistry, context, "X"));
  ASSERT_EQ(0u, context.properties.size());
}

TEST_F(PropertySystemTest, RenameProperty)
{
  auto& context = mRegistry.ctx<PropertyContext>();

  Sys::AddProperty(mRegistry, context, "abc", PropertyType::Color);
  ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "abc"));

  Sys::RenameProperty(mRegistry, context, "abc", "def");
  ASSERT_FALSE(Sys::HasPropertyWithName(mRegistry, context, "abc"));
  ASSERT_TRUE(Sys::HasPropertyWithName(mRegistry, context, "def"));
}

TEST_F(PropertySystemTest, UpdateProperty)
{
  auto& context = mRegistry.ctx<PropertyContext>();

  Sys::AddProperty(mRegistry, context, "xyz", 45.3f);
  ASSERT_EQ(45.3f, Sys::GetProperty(mRegistry, context, "xyz").value.AsFloat());

  Sys::UpdateProperty(mRegistry, context, "xyz", 123.5f);
  ASSERT_EQ(123.5f, Sys::GetProperty(mRegistry, context, "xyz").value.AsFloat());
}

TEST_F(PropertySystemTest, ChangePropertyType)
{
  auto& context = mRegistry.ctx<PropertyContext>();

  Sys::AddProperty(mRegistry, context, "foo", 123);
  ASSERT_TRUE(Sys::GetProperty(mRegistry, context, "foo").value.IsInt());

  Sys::ChangePropertyType(mRegistry, context, "foo", PropertyType::String);
  ASSERT_TRUE(Sys::GetProperty(mRegistry, context, "foo").value.IsString());
  ASSERT_EQ("", Sys::GetProperty(mRegistry, context, "foo").value.AsString());

  Sys::ChangePropertyType(mRegistry, context, "foo", PropertyType::Integer);
  ASSERT_TRUE(Sys::GetProperty(mRegistry, context, "foo").value.IsInt());
  ASSERT_EQ(0, Sys::GetProperty(mRegistry, context, "foo").value.AsInt());
}

TEST_F(PropertySystemTest, GetContext)
{
  ASSERT_NO_THROW(Sys::GetContext(mRegistry, mRegistry.ctx<PropertyContext>().id));
  ASSERT_THROW(Sys::GetContext(mRegistry, Sys::GetNextContextId()), TactileError);
}

TEST_F(PropertySystemTest, GetProperty)
{
  auto& context = mRegistry.ctx<PropertyContext>();
  ASSERT_THROW(Sys::GetProperty(mRegistry, context, "foo"), TactileError);

  Sys::AddProperty(mRegistry, context, "foo", 42);
  const auto& property = Sys::GetProperty(mRegistry, context, "foo");

  ASSERT_EQ("foo", property.name);
  ASSERT_EQ(42, property.value.AsInt());
}