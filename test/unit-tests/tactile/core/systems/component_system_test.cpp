#include "core/systems/component_system.hpp"

#include <string>  // string

#include <gtest/gtest.h>
#include <tactile_stdlib.hpp>

#include "core/systems/registry_system.hpp"

using namespace Tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

[[nodiscard]] auto CreateEntityWithBundle(entt::registry& registry) -> entt::entity
{
  const auto entity = registry.create();
  registry.emplace<ComponentBundle>(entity);
  return entity;
}

}  // namespace

TEST(ComponentSystem, CreateComponentDef)
{
  auto registry = Sys::MakeRegistry();
  ASSERT_FALSE(Sys::IsComponentNameTaken(registry, "Foo"));
  ASSERT_FALSE(Sys::IsComponentNameTaken(registry, "Bar"));

  const auto foo = Sys::CreateComponentDef(registry, "Foo");
  ASSERT_TRUE(Sys::IsComponentNameTaken(registry, "Foo"));
  ASSERT_FALSE(Sys::IsComponentNameTaken(registry, "Bar"));

  const auto bar = Sys::CreateComponentDef(registry, "Bar");
  ASSERT_TRUE(Sys::IsComponentNameTaken(registry, "Foo"));
  ASSERT_TRUE(Sys::IsComponentNameTaken(registry, "Bar"));

  ASSERT_NE(foo, bar);

  ASSERT_EQ("Foo", Sys::GetComponentDefName(registry, foo));
  ASSERT_EQ("Bar", Sys::GetComponentDefName(registry, bar));

  const auto [entity, def] = Sys::GetComponentDef(registry, foo);
  ASSERT_EQ(foo, def.id);
  ASSERT_EQ("Foo", def.name);
  ASSERT_TRUE(def.attributes.empty());
}

TEST(ComponentSystem, RemoveComponentDef)
{
  const std::string name = "Definition";

  auto registry = Sys::MakeRegistry();

  const auto def = Sys::CreateComponentDef(registry, name);
  ASSERT_TRUE(Sys::IsComponentNameTaken(registry, name));
  ASSERT_NE(null_entity, Sys::FindComponentDef(registry, def));

  const auto a = CreateEntityWithBundle(registry);
  const auto b = CreateEntityWithBundle(registry);
  const auto c = CreateEntityWithBundle(registry);

  Sys::AddComponent(registry, a, def);
  Sys::AddComponent(registry, b, def);
  Sys::AddComponent(registry, c, def);

  ASSERT_TRUE(Sys::HasComponent(registry, a, def));
  ASSERT_TRUE(Sys::HasComponent(registry, b, def));
  ASSERT_TRUE(Sys::HasComponent(registry, c, def));

  Sys::RemoveComponentDef(registry, def);

  ASSERT_FALSE(Sys::IsComponentNameTaken(registry, name));
  ASSERT_EQ(null_entity, Sys::FindComponentDef(registry, def));

  ASSERT_FALSE(Sys::HasComponent(registry, a, def));
  ASSERT_FALSE(Sys::HasComponent(registry, b, def));
  ASSERT_FALSE(Sys::HasComponent(registry, c, def));
}

TEST(ComponentSystem, RenameComponentDef)
{
  auto registry = Sys::MakeRegistry();

  const auto def = Sys::CreateComponentDef(registry, "Foo");
  ASSERT_EQ("Foo", Sys::GetComponentDefName(registry, def));

  Sys::RenameComponentDef(registry, def, "Bar");
  ASSERT_EQ("Bar", Sys::GetComponentDefName(registry, def));
}

TEST(ComponentSystem, FindComponentDef)
{
  auto registry = Sys::MakeRegistry();
  const auto id = Sys::CreateComponentDef(registry, "Foo");
  ASSERT_NE(null_entity, Sys::FindComponentDef(registry, id));
  ASSERT_EQ(null_entity, Sys::FindComponentDef(registry, id + 1));
}

TEST(ComponentSystem, GetComponentDef)
{
  auto registry = Sys::MakeRegistry();
  const auto id = Sys::CreateComponentDef(registry, "Foo");

  const auto& ref = registry;

  ASSERT_THROW(Sys::GetComponentDef(registry, id + 1), TactileError);
  ASSERT_THROW(Sys::GetComponentDef(ref, id + 1), TactileError);

  {
    const auto [entity, def] = Sys::GetComponentDef(registry, id);
    ASSERT_EQ(id, def.id);
    ASSERT_EQ("Foo", def.name);
  }

  {
    const auto [entity, def] = Sys::GetComponentDef(ref, id);
    ASSERT_EQ(id, def.id);
    ASSERT_EQ("Foo", def.name);
  }
}

TEST(ComponentSystem, GetComponentDefName)
{
  auto registry = Sys::MakeRegistry();
  const auto id = Sys::CreateComponentDef(registry, "Hello");
  ASSERT_EQ("Hello", Sys::GetComponentDefName(registry, id));
}

TEST(ComponentSystem, CreateComponentAttribute)
{
  auto registry = Sys::MakeRegistry();
  const auto def = Sys::CreateComponentDef(registry, "Def");
  ASSERT_FALSE(Sys::IsComponentAttributeNameTaken(registry, def, "A"));

  Sys::CreateComponentAttribute(registry, def, "A");
  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "A"));
}

TEST(ComponentSystem, RemoveComponentAttribute)
{
  auto registry = Sys::MakeRegistry();
  const auto def = Sys::CreateComponentDef(registry, "Def");

  const auto a = CreateEntityWithBundle(registry);
  const auto b = CreateEntityWithBundle(registry);

  Sys::AddComponent(registry, a, def);
  Sys::AddComponent(registry, b, def);

  Sys::CreateComponentAttribute(registry, def, "A");
  Sys::CreateComponentAttribute(registry, def, "B");

  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "B"));
  ASSERT_TRUE(Sys::HasComponent(registry, a, def));
  ASSERT_TRUE(Sys::HasComponent(registry, b, def));
  ASSERT_EQ(1u, Sys::GetComponentCount(registry, a));
  ASSERT_EQ(1u, Sys::GetComponentCount(registry, b));
  ASSERT_EQ(2u, Sys::GetComponent(registry, a, def).values.size());

  Sys::RemoveComponentAttribute(registry, def, "A");

  ASSERT_FALSE(Sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "B"));
  ASSERT_TRUE(Sys::HasComponent(registry, a, def));
  ASSERT_TRUE(Sys::HasComponent(registry, b, def));
  ASSERT_EQ(1u, Sys::GetComponentCount(registry, a));
  ASSERT_EQ(1u, Sys::GetComponentCount(registry, b));
  ASSERT_EQ(1u, Sys::GetComponent(registry, a, def).values.size());
}

TEST(ComponentSystem, RenameComponentAttribute)
{
  auto registry = Sys::MakeRegistry();
  const auto def = Sys::CreateComponentDef(registry, "Def");

  Sys::CreateComponentAttribute(registry, def, "A");

  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_FALSE(Sys::IsComponentAttributeNameTaken(registry, def, "B"));

  Sys::RenameComponentAttribute(registry, def, "A", "B");

  ASSERT_FALSE(Sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "B"));
}

TEST(ComponentSystem, SetComponentAttributeType)
{
  auto registry = Sys::MakeRegistry();
  const auto def = Sys::CreateComponentDef(registry, "Def");

  Sys::CreateComponentAttribute(registry, def, "A");
  ASSERT_EQ(PropertyType::String, Sys::GetComponentAttributeType(registry, def, "A"));

  Sys::SetComponentAttributeType(registry, def, "A", PropertyType::Integer);
  ASSERT_EQ(PropertyType::Integer, Sys::GetComponentAttributeType(registry, def, "A"));

  ASSERT_THROW(Sys::GetComponentAttributeType(registry, def, "B"), TactileError);
}

TEST(ComponentSystem, SetComponentAttributeValue)
{
  using namespace std::string_literals;

  auto registry = Sys::MakeRegistry();
  const auto def = Sys::CreateComponentDef(registry, "Def");

  ASSERT_THROW(Sys::SetComponentAttributeValue(registry, def, "ABC", 42), TactileError);

  Sys::CreateComponentAttribute(registry, def, "Foo");
  Sys::SetComponentAttributeValue(registry, def, "Foo", "Bar"s);

  ASSERT_EQ("Bar", Sys::GetComponentAttributeValue(registry, def, "Foo").AsString());

  Sys::SetComponentAttributeType(registry, def, "Foo", PropertyType::Boolean);
  Sys::SetComponentAttributeValue(registry, def, "Foo", true);

  ASSERT_TRUE(Sys::GetComponentAttributeValue(registry, def, "Foo").AsBool());
}

TEST(ComponentSystem, IsComponentAttributeNameTaken)
{
  auto registry = Sys::MakeRegistry();
  const auto def = Sys::CreateComponentDef(registry, "Def");
  ASSERT_FALSE(Sys::IsComponentAttributeNameTaken(registry, def, "abc"));

  Sys::CreateComponentAttribute(registry, def, "abc");
  ASSERT_TRUE(Sys::IsComponentAttributeNameTaken(registry, def, "abc"));
}

TEST(ComponentSystem, AddComponent)
{
  auto registry = Sys::MakeRegistry();

  const auto def = Sys::CreateComponentDef(registry, "Position");
  Sys::CreateComponentAttribute(registry, def, "X");
  Sys::CreateComponentAttribute(registry, def, "Y");

  Sys::SetComponentAttributeType(registry, def, "X", PropertyType::Integer);
  Sys::SetComponentAttributeValue(registry, def, "X", 42);

  Sys::SetComponentAttributeType(registry, def, "Y", PropertyType::Floating);
  Sys::SetComponentAttributeValue(registry, def, "Y", -3.5f);

  const auto entity = CreateEntityWithBundle(registry);
  auto& component = Sys::AddComponent(registry, entity, def);
  ASSERT_EQ(def, component.type);
  ASSERT_EQ(2u, component.values.size());

  ASSERT_EQ(42, Sys::GetComponentAttribute(registry, entity, def, "X").AsInt());
  ASSERT_EQ(-3.5f, Sys::GetComponentAttribute(registry, entity, def, "Y").AsFloat());

  ASSERT_THROW(Sys::GetComponentAttribute(registry, entity, def, "foo"), TactileError);
}

TEST(ComponentSystem, HasComponent)
{
  auto registry = Sys::MakeRegistry();

  const auto a = CreateEntityWithBundle(registry);
  const auto b = CreateEntityWithBundle(registry);

  const auto def = Sys::CreateComponentDef(registry, "def");

  ASSERT_FALSE(Sys::HasComponent(registry, a, def));
  ASSERT_FALSE(Sys::HasComponent(registry, b, def));

  Sys::AddComponent(registry, a, def);

  ASSERT_TRUE(Sys::HasComponent(registry, a, def));
  ASSERT_FALSE(Sys::HasComponent(registry, b, def));

  Sys::RemoveComponent(registry, a, def);

  ASSERT_FALSE(Sys::HasComponent(registry, a, def));
  ASSERT_FALSE(Sys::HasComponent(registry, b, def));
}

TEST(ComponentSystem, RemoveComponent)
{
  auto registry = Sys::MakeRegistry();

  const auto a = Sys::CreateComponentDef(registry, "A");
  const auto b = Sys::CreateComponentDef(registry, "B");

  const auto entity = CreateEntityWithBundle(registry);
  Sys::AddComponent(registry, entity, a);
  Sys::AddComponent(registry, entity, b);

  ASSERT_TRUE(Sys::HasComponent(registry, entity, a));
  ASSERT_TRUE(Sys::HasComponent(registry, entity, b));
  ASSERT_EQ(2u, Sys::GetComponentCount(registry, entity));

  Sys::RemoveComponent(registry, entity, b);
  ASSERT_TRUE(Sys::HasComponent(registry, entity, a));
  ASSERT_FALSE(Sys::HasComponent(registry, entity, b));
  ASSERT_EQ(1u, Sys::GetComponentCount(registry, entity));

  Sys::RemoveComponent(registry, entity, a);
  ASSERT_FALSE(Sys::HasComponent(registry, entity, a));
  ASSERT_FALSE(Sys::HasComponent(registry, entity, b));
  ASSERT_EQ(0u, Sys::GetComponentCount(registry, entity));
}

TEST(ComponentSystem, GetComponent)
{
  auto registry = Sys::MakeRegistry();

  const auto a = Sys::CreateComponentDef(registry, "A");
  const auto b = Sys::CreateComponentDef(registry, "B");

  const auto entity = CreateEntityWithBundle(registry);
  ASSERT_THROW(Sys::GetComponent(registry, entity, a), TactileError);
  ASSERT_THROW(Sys::GetComponent(registry, entity, b), TactileError);

  Sys::AddComponent(registry, entity, a);
  ASSERT_NO_THROW(Sys::GetComponent(registry, entity, a));
  ASSERT_THROW(Sys::GetComponent(registry, entity, b), TactileError);

  const auto& component = Sys::GetComponent(registry, entity, a);
  ASSERT_EQ(a, component.type);
}
