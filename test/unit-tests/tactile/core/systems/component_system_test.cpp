#include "core/systems/component_system.hpp"

#include <string>  // string

#include <gtest/gtest.h>
#include <tactile_stdlib.hpp>

#include "core/components/property_context.hpp"
#include "core/systems/registry_system.hpp"
#include "core/systems/property_system.hpp"

using namespace tactile;

namespace {

constexpr entt::entity null_entity = entt::null;

[[nodiscard]] auto CreateContext(entt::registry& registry) -> ContextID
{
  const auto entity = registry.create();
  const auto& context = sys::AddPropertyContext(registry, entity);
  return context.id;
}

}  // namespace

TEST(ComponentSystem, CreateComponentDef)
{
  auto registry = sys::MakeRegistry();
  ASSERT_FALSE(sys::IsComponentNameTaken(registry, "Foo"));
  ASSERT_FALSE(sys::IsComponentNameTaken(registry, "Bar"));

  const auto foo = sys::CreateComponentDef(registry, "Foo");
  ASSERT_TRUE(sys::IsComponentNameTaken(registry, "Foo"));
  ASSERT_FALSE(sys::IsComponentNameTaken(registry, "Bar"));

  const auto bar = sys::CreateComponentDef(registry, "Bar");
  ASSERT_TRUE(sys::IsComponentNameTaken(registry, "Foo"));
  ASSERT_TRUE(sys::IsComponentNameTaken(registry, "Bar"));

  ASSERT_NE(foo, bar);

  ASSERT_EQ("Foo", sys::GetComponentDefName(registry, foo));
  ASSERT_EQ("Bar", sys::GetComponentDefName(registry, bar));

  const auto [entity, def] = sys::GetComponentDef(registry, foo);
  ASSERT_EQ(foo, def.id);
  ASSERT_EQ("Foo", def.name);
  ASSERT_TRUE(def.attributes.empty());
}

TEST(ComponentSystem, RemoveComponentDef)
{
  const std::string name = "Definition";

  auto registry = sys::MakeRegistry();

  const auto def = sys::CreateComponentDef(registry, name);
  ASSERT_TRUE(sys::IsComponentNameTaken(registry, name));
  ASSERT_NE(null_entity, sys::FindComponentDef(registry, def));

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);
  const auto c = CreateContext(registry);

  sys::AddComponent(registry, a, def);
  sys::AddComponent(registry, b, def);
  sys::AddComponent(registry, c, def);

  ASSERT_TRUE(sys::HasComponent(registry, a, def));
  ASSERT_TRUE(sys::HasComponent(registry, b, def));
  ASSERT_TRUE(sys::HasComponent(registry, c, def));

  sys::RemoveComponentDef(registry, def);

  ASSERT_FALSE(sys::IsComponentNameTaken(registry, name));
  ASSERT_EQ(null_entity, sys::FindComponentDef(registry, def));

  ASSERT_FALSE(sys::HasComponent(registry, a, def));
  ASSERT_FALSE(sys::HasComponent(registry, b, def));
  ASSERT_FALSE(sys::HasComponent(registry, c, def));
}

TEST(ComponentSystem, RenameComponentDef)
{
  auto registry = sys::MakeRegistry();

  const auto def = sys::CreateComponentDef(registry, "Foo");
  ASSERT_EQ("Foo", sys::GetComponentDefName(registry, def));

  sys::RenameComponentDef(registry, def, "Bar");
  ASSERT_EQ("Bar", sys::GetComponentDefName(registry, def));
}

TEST(ComponentSystem, FindComponentDef)
{
  auto registry = sys::MakeRegistry();
  const auto id = sys::CreateComponentDef(registry, "Foo");
  ASSERT_NE(null_entity, sys::FindComponentDef(registry, id));
  ASSERT_EQ(null_entity, sys::FindComponentDef(registry, id + 1));
}

TEST(ComponentSystem, GetComponentDef)
{
  auto registry = sys::MakeRegistry();
  const auto id = sys::CreateComponentDef(registry, "Foo");

  const auto& ref = registry;

  ASSERT_THROW(sys::GetComponentDef(registry, id + 1), TactileError);
  ASSERT_THROW(sys::GetComponentDef(ref, id + 1), TactileError);

  {
    const auto [entity, def] = sys::GetComponentDef(registry, id);
    ASSERT_EQ(id, def.id);
    ASSERT_EQ("Foo", def.name);
  }

  {
    const auto [entity, def] = sys::GetComponentDef(ref, id);
    ASSERT_EQ(id, def.id);
    ASSERT_EQ("Foo", def.name);
  }
}

TEST(ComponentSystem, GetComponentDefName)
{
  auto registry = sys::MakeRegistry();
  const auto id = sys::CreateComponentDef(registry, "Hello");
  ASSERT_EQ("Hello", sys::GetComponentDefName(registry, id));
}

TEST(ComponentSystem, CreateComponentAttribute)
{
  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");
  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "A"));

  sys::CreateComponentAttribute(registry, def, "A");
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
}

TEST(ComponentSystem, RemoveComponentAttribute)
{
  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);

  sys::AddComponent(registry, a, def);
  sys::AddComponent(registry, b, def);

  sys::CreateComponentAttribute(registry, def, "A");
  sys::CreateComponentAttribute(registry, def, "B");

  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "B"));
  ASSERT_TRUE(sys::HasComponent(registry, a, def));
  ASSERT_TRUE(sys::HasComponent(registry, b, def));
  ASSERT_EQ(1u, sys::GetComponentCount(registry, a));
  ASSERT_EQ(1u, sys::GetComponentCount(registry, b));
  ASSERT_EQ(2u, sys::GetComponent(registry, a, def).values.size());

  sys::RemoveComponentAttribute(registry, def, "A");

  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "B"));
  ASSERT_TRUE(sys::HasComponent(registry, a, def));
  ASSERT_TRUE(sys::HasComponent(registry, b, def));
  ASSERT_EQ(1u, sys::GetComponentCount(registry, a));
  ASSERT_EQ(1u, sys::GetComponentCount(registry, b));
  ASSERT_EQ(1u, sys::GetComponent(registry, a, def).values.size());
}

TEST(ComponentSystem, RenameComponentAttribute)
{
  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");

  sys::CreateComponentAttribute(registry, def, "A");

  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "B"));

  sys::RenameComponentAttribute(registry, def, "A", "B");

  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "B"));
}

TEST(ComponentSystem, DuplicateComponentAttribute)
{
  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");

  sys::CreateComponentAttribute(registry, def, "A");
  ASSERT_EQ(1u, sys::GetComponentAttributeCount(registry, def));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "A (1)"));

  sys::DuplicateComponentAttribute(registry, def, "A");
  ASSERT_EQ(2u, sys::GetComponentAttributeCount(registry, def));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "A"));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "A (1)"));

  sys::CreateComponentAttribute(registry, def, "B");
  sys::CreateComponentAttribute(registry, def, "B (1)");
  ASSERT_EQ(4u, sys::GetComponentAttributeCount(registry, def));
  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "B (2)"));

  sys::DuplicateComponentAttribute(registry, def, "B");
  ASSERT_EQ(5u, sys::GetComponentAttributeCount(registry, def));
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "B (2)"));
}

TEST(ComponentSystem, SetComponentAttributeType)
{
  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");

  sys::CreateComponentAttribute(registry, def, "A");
  ASSERT_EQ(PropertyType::String, sys::GetComponentAttributeType(registry, def, "A"));

  sys::SetComponentAttributeType(registry, def, "A", PropertyType::Integer);
  ASSERT_EQ(PropertyType::Integer, sys::GetComponentAttributeType(registry, def, "A"));

  ASSERT_THROW(sys::GetComponentAttributeType(registry, def, "B"), TactileError);
}

TEST(ComponentSystem, SetComponentAttributeValue)
{
  using namespace std::string_literals;

  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");

  ASSERT_THROW(sys::SetComponentAttributeValue(registry, def, "ABC", 42), TactileError);

  sys::CreateComponentAttribute(registry, def, "Foo");
  sys::SetComponentAttributeValue(registry, def, "Foo", "Bar"s);

  ASSERT_EQ("Bar", sys::GetComponentAttributeValue(registry, def, "Foo").as_string());

  sys::SetComponentAttributeType(registry, def, "Foo", PropertyType::Boolean);
  sys::SetComponentAttributeValue(registry, def, "Foo", true);

  ASSERT_TRUE(sys::GetComponentAttributeValue(registry, def, "Foo").as_bool());
}

TEST(ComponentSystem, IsComponentAttributeNameTaken)
{
  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Def");
  ASSERT_FALSE(sys::IsComponentAttributeNameTaken(registry, def, "abc"));

  sys::CreateComponentAttribute(registry, def, "abc");
  ASSERT_TRUE(sys::IsComponentAttributeNameTaken(registry, def, "abc"));
}

TEST(ComponentSystem, AddComponent)
{
  auto registry = sys::MakeRegistry();

  const auto def = sys::CreateComponentDef(registry, "Position");
  sys::CreateComponentAttribute(registry, def, "X");
  sys::CreateComponentAttribute(registry, def, "Y");

  sys::SetComponentAttributeType(registry, def, "X", PropertyType::Integer);
  sys::SetComponentAttributeValue(registry, def, "X", 42);

  sys::SetComponentAttributeType(registry, def, "Y", PropertyType::Floating);
  sys::SetComponentAttributeValue(registry, def, "Y", -3.5f);

  const auto entity = CreateContext(registry);
  auto& component = sys::AddComponent(registry, entity, def);
  ASSERT_EQ(def, component.type);
  ASSERT_EQ(2u, component.values.size());

  ASSERT_EQ(42, sys::GetComponentAttribute(registry, entity, def, "X").as_int());
  ASSERT_EQ(-3.5f, sys::GetComponentAttribute(registry, entity, def, "Y").as_float());

  ASSERT_THROW(sys::GetComponentAttribute(registry, entity, def, "foo"), TactileError);
}

TEST(ComponentSystem, ResetComponent)
{
  using namespace std::string_literals;

  auto registry = sys::MakeRegistry();
  const auto def = sys::CreateComponentDef(registry, "Foo");

  sys::CreateComponentAttribute(registry, def, "A", 42);
  sys::CreateComponentAttribute(registry, def, "B", "Boo"s);
  sys::CreateComponentAttribute(registry, def, "C", 1.5f);

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);

  sys::AddComponent(registry, a, def);
  sys::AddComponent(registry, b, def);

  sys::UpdateComponent(registry, b, def, "A", 123);
  sys::UpdateComponent(registry, b, def, "B", "Coo"s);
  sys::UpdateComponent(registry, b, def, "C", 2.0f);

  ASSERT_TRUE(sys::HasComponent(registry, a, def));
  ASSERT_TRUE(sys::HasComponent(registry, b, def));

  ASSERT_EQ(42, sys::GetComponentAttribute(registry, a, def, "A").as_int());
  ASSERT_EQ(123, sys::GetComponentAttribute(registry, b, def, "A").as_int());

  ASSERT_EQ("Boo", sys::GetComponentAttribute(registry, a, def, "B").as_string());
  ASSERT_EQ("Coo", sys::GetComponentAttribute(registry, b, def, "B").as_string());

  ASSERT_EQ(1.5f, sys::GetComponentAttribute(registry, a, def, "C").as_float());
  ASSERT_EQ(2.0f, sys::GetComponentAttribute(registry, b, def, "C").as_float());

  sys::ResetComponent(registry, b, def);

  ASSERT_TRUE(sys::HasComponent(registry, a, def));
  ASSERT_TRUE(sys::HasComponent(registry, b, def));

  ASSERT_EQ(42, sys::GetComponentAttribute(registry, a, def, "A").as_int());
  ASSERT_EQ(42, sys::GetComponentAttribute(registry, b, def, "A").as_int());

  ASSERT_EQ("Boo", sys::GetComponentAttribute(registry, a, def, "B").as_string());
  ASSERT_EQ("Boo", sys::GetComponentAttribute(registry, b, def, "B").as_string());

  ASSERT_EQ(1.5f, sys::GetComponentAttribute(registry, a, def, "C").as_float());
  ASSERT_EQ(1.5f, sys::GetComponentAttribute(registry, b, def, "C").as_float());
}

TEST(ComponentSystem, HasComponent)
{
  auto registry = sys::MakeRegistry();

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);

  const auto def = sys::CreateComponentDef(registry, "def");

  ASSERT_FALSE(sys::HasComponent(registry, a, def));
  ASSERT_FALSE(sys::HasComponent(registry, b, def));

  sys::AddComponent(registry, a, def);

  ASSERT_TRUE(sys::HasComponent(registry, a, def));
  ASSERT_FALSE(sys::HasComponent(registry, b, def));

  sys::RemoveComponent(registry, a, def);

  ASSERT_FALSE(sys::HasComponent(registry, a, def));
  ASSERT_FALSE(sys::HasComponent(registry, b, def));
}

TEST(ComponentSystem, RemoveComponent)
{
  auto registry = sys::MakeRegistry();

  const auto a = sys::CreateComponentDef(registry, "A");
  const auto b = sys::CreateComponentDef(registry, "B");

  const auto entity = CreateContext(registry);
  sys::AddComponent(registry, entity, a);
  sys::AddComponent(registry, entity, b);

  ASSERT_TRUE(sys::HasComponent(registry, entity, a));
  ASSERT_TRUE(sys::HasComponent(registry, entity, b));
  ASSERT_EQ(2u, sys::GetComponentCount(registry, entity));

  sys::RemoveComponent(registry, entity, b);
  ASSERT_TRUE(sys::HasComponent(registry, entity, a));
  ASSERT_FALSE(sys::HasComponent(registry, entity, b));
  ASSERT_EQ(1u, sys::GetComponentCount(registry, entity));

  sys::RemoveComponent(registry, entity, a);
  ASSERT_FALSE(sys::HasComponent(registry, entity, a));
  ASSERT_FALSE(sys::HasComponent(registry, entity, b));
  ASSERT_EQ(0u, sys::GetComponentCount(registry, entity));
}

TEST(ComponentSystem, GetComponent)
{
  auto registry = sys::MakeRegistry();

  const auto a = sys::CreateComponentDef(registry, "A");
  const auto b = sys::CreateComponentDef(registry, "B");

  const auto entity = CreateContext(registry);
  ASSERT_THROW(sys::GetComponent(registry, entity, a), TactileError);
  ASSERT_THROW(sys::GetComponent(registry, entity, b), TactileError);

  sys::AddComponent(registry, entity, a);
  ASSERT_NO_THROW(sys::GetComponent(registry, entity, a));
  ASSERT_THROW(sys::GetComponent(registry, entity, b), TactileError);

  const auto& component = sys::GetComponent(registry, entity, a);
  ASSERT_EQ(a, component.type);
}
