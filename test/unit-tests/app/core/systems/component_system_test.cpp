#include "core/systems/component_system.hpp"

#include <string>  // string

#include <gtest/gtest.h>
#include <tactile_stdlib.hpp>

#include "core/components/attribute_context.hpp"
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
  auto registry = sys::make_document_registry();
  ASSERT_FALSE(sys::is_component_name_taken(registry, "Foo"));
  ASSERT_FALSE(sys::is_component_name_taken(registry, "Bar"));

  const auto foo = sys::make_component_def(registry, "Foo");
  ASSERT_TRUE(sys::is_component_name_taken(registry, "Foo"));
  ASSERT_FALSE(sys::is_component_name_taken(registry, "Bar"));

  const auto bar = sys::make_component_def(registry, "Bar");
  ASSERT_TRUE(sys::is_component_name_taken(registry, "Foo"));
  ASSERT_TRUE(sys::is_component_name_taken(registry, "Bar"));

  ASSERT_NE(foo, bar);

  ASSERT_EQ("Foo", sys::get_component_def_name(registry, foo));
  ASSERT_EQ("Bar", sys::get_component_def_name(registry, bar));

  const auto [entity, def] = sys::get_component_def(registry, foo);
  ASSERT_EQ(foo, def.id);
  ASSERT_EQ("Foo", def.name);
  ASSERT_TRUE(def.attributes.empty());
}

TEST(ComponentSystem, RemoveComponentDef)
{
  const std::string name = "Definition";

  auto registry = sys::make_document_registry();

  const auto def = sys::make_component_def(registry, name);
  ASSERT_TRUE(sys::is_component_name_taken(registry, name));
  ASSERT_NE(null_entity, sys::find_component_def(registry, def));

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);
  const auto c = CreateContext(registry);

  sys::add_component(registry, a, def);
  sys::add_component(registry, b, def);
  sys::add_component(registry, c, def);

  ASSERT_TRUE(sys::has_component(registry, a, def));
  ASSERT_TRUE(sys::has_component(registry, b, def));
  ASSERT_TRUE(sys::has_component(registry, c, def));

  sys::remove_component_def(registry, def);

  ASSERT_FALSE(sys::is_component_name_taken(registry, name));
  ASSERT_EQ(null_entity, sys::find_component_def(registry, def));

  ASSERT_FALSE(sys::has_component(registry, a, def));
  ASSERT_FALSE(sys::has_component(registry, b, def));
  ASSERT_FALSE(sys::has_component(registry, c, def));
}

TEST(ComponentSystem, RenameComponentDef)
{
  auto registry = sys::make_document_registry();

  const auto def = sys::make_component_def(registry, "Foo");
  ASSERT_EQ("Foo", sys::get_component_def_name(registry, def));

  sys::rename_component_def(registry, def, "Bar");
  ASSERT_EQ("Bar", sys::get_component_def_name(registry, def));
}

TEST(ComponentSystem, FindComponentDef)
{
  auto registry = sys::make_document_registry();
  const auto id = sys::make_component_def(registry, "Foo");
  ASSERT_NE(null_entity, sys::find_component_def(registry, id));
  ASSERT_EQ(null_entity, sys::find_component_def(registry, id + 1));
}

TEST(ComponentSystem, GetComponentDef)
{
  auto registry = sys::make_document_registry();
  const auto id = sys::make_component_def(registry, "Foo");

  const auto& ref = registry;

  ASSERT_THROW(sys::get_component_def(registry, id + 1), tactile_error);
  ASSERT_THROW(sys::get_component_def(ref, id + 1), tactile_error);

  {
    const auto [entity, def] = sys::get_component_def(registry, id);
    ASSERT_EQ(id, def.id);
    ASSERT_EQ("Foo", def.name);
  }

  {
    const auto [entity, def] = sys::get_component_def(ref, id);
    ASSERT_EQ(id, def.id);
    ASSERT_EQ("Foo", def.name);
  }
}

TEST(ComponentSystem, GetComponentDefName)
{
  auto registry = sys::make_document_registry();
  const auto id = sys::make_component_def(registry, "Hello");
  ASSERT_EQ("Hello", sys::get_component_def_name(registry, id));
}

TEST(ComponentSystem, CreateComponentAttribute)
{
  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");
  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "A"));

  sys::make_component_attribute(registry, def, "A");
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "A"));
}

TEST(ComponentSystem, RemoveComponentAttribute)
{
  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);

  sys::add_component(registry, a, def);
  sys::add_component(registry, b, def);

  sys::make_component_attribute(registry, def, "A");
  sys::make_component_attribute(registry, def, "B");

  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "A"));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "B"));
  ASSERT_TRUE(sys::has_component(registry, a, def));
  ASSERT_TRUE(sys::has_component(registry, b, def));
  ASSERT_EQ(1u, sys::get_component_count(registry, a));
  ASSERT_EQ(1u, sys::get_component_count(registry, b));
  ASSERT_EQ(2u, sys::get_component(registry, a, def).values.size());

  sys::remove_component_attribute(registry, def, "A");

  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "A"));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "B"));
  ASSERT_TRUE(sys::has_component(registry, a, def));
  ASSERT_TRUE(sys::has_component(registry, b, def));
  ASSERT_EQ(1u, sys::get_component_count(registry, a));
  ASSERT_EQ(1u, sys::get_component_count(registry, b));
  ASSERT_EQ(1u, sys::get_component(registry, a, def).values.size());
}

TEST(ComponentSystem, RenameComponentAttribute)
{
  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");

  sys::make_component_attribute(registry, def, "A");

  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "A"));
  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "B"));

  sys::rename_component_attribute(registry, def, "A", "B");

  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "A"));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "B"));
}

TEST(ComponentSystem, DuplicateComponentAttribute)
{
  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");

  sys::make_component_attribute(registry, def, "A");
  ASSERT_EQ(1u, sys::get_component_attribute_count(registry, def));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "A"));
  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "A (1)"));

  sys::duplicate_component_attribute(registry, def, "A");
  ASSERT_EQ(2u, sys::get_component_attribute_count(registry, def));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "A"));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "A (1)"));

  sys::make_component_attribute(registry, def, "B");
  sys::make_component_attribute(registry, def, "B (1)");
  ASSERT_EQ(4u, sys::get_component_attribute_count(registry, def));
  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "B (2)"));

  sys::duplicate_component_attribute(registry, def, "B");
  ASSERT_EQ(5u, sys::get_component_attribute_count(registry, def));
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "B (2)"));
}

TEST(ComponentSystem, SetComponentAttributeType)
{
  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");

  sys::make_component_attribute(registry, def, "A");
  ASSERT_EQ(attribute_type::string, sys::get_component_attribute_type(registry, def, "A"));

  sys::set_component_attribute_type(registry, def, "A", attribute_type::integer);
  ASSERT_EQ(attribute_type::integer, sys::get_component_attribute_type(registry, def, "A"));

  ASSERT_THROW(sys::get_component_attribute_type(registry, def, "B"), tactile_error);
}

TEST(ComponentSystem, SetComponentAttributeValue)
{
  using namespace std::string_literals;

  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");

  ASSERT_THROW(sys::set_component_attribute_value(registry, def, "ABC", 42),
               tactile_error);

  sys::make_component_attribute(registry, def, "Foo");
  sys::set_component_attribute_value(registry, def, "Foo", "Bar"s);

  ASSERT_EQ("Bar", sys::get_component_attribute_value(registry, def, "Foo").as_string());

  sys::set_component_attribute_type(registry, def, "Foo", attribute_type::boolean);
  sys::set_component_attribute_value(registry, def, "Foo", true);

  ASSERT_TRUE(sys::get_component_attribute_value(registry, def, "Foo").as_bool());
}

TEST(ComponentSystem, IsComponentAttributeNameTaken)
{
  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Def");
  ASSERT_FALSE(sys::is_component_attribute_name_taken(registry, def, "abc"));

  sys::make_component_attribute(registry, def, "abc");
  ASSERT_TRUE(sys::is_component_attribute_name_taken(registry, def, "abc"));
}

TEST(ComponentSystem, AddComponent)
{
  auto registry = sys::make_document_registry();

  const auto def = sys::make_component_def(registry, "Position");
  sys::make_component_attribute(registry, def, "X");
  sys::make_component_attribute(registry, def, "Y");

  sys::set_component_attribute_type(registry, def, "X", attribute_type::integer);
  sys::set_component_attribute_value(registry, def, "X", 42);

  sys::set_component_attribute_type(registry, def, "Y", attribute_type::floating);
  sys::set_component_attribute_value(registry, def, "Y", -3.5f);

  const auto entity = CreateContext(registry);
  auto& component = sys::add_component(registry, entity, def);
  ASSERT_EQ(def, component.type);
  ASSERT_EQ(2u, component.values.size());

  ASSERT_EQ(42, sys::get_component_attribute(registry, entity, def, "X").as_int());
  ASSERT_EQ(-3.5f, sys::get_component_attribute(registry, entity, def, "Y").as_float());

  ASSERT_THROW(sys::get_component_attribute(registry, entity, def, "foo"), tactile_error);
}

TEST(ComponentSystem, ResetComponent)
{
  using namespace std::string_literals;

  auto registry = sys::make_document_registry();
  const auto def = sys::make_component_def(registry, "Foo");

  sys::make_component_attribute(registry, def, "A", 42);
  sys::make_component_attribute(registry, def, "B", "Boo"s);
  sys::make_component_attribute(registry, def, "C", 1.5f);

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);

  sys::add_component(registry, a, def);
  sys::add_component(registry, b, def);

  sys::update_component(registry, b, def, "A", 123);
  sys::update_component(registry, b, def, "B", "Coo"s);
  sys::update_component(registry, b, def, "C", 2.0f);

  ASSERT_TRUE(sys::has_component(registry, a, def));
  ASSERT_TRUE(sys::has_component(registry, b, def));

  ASSERT_EQ(42, sys::get_component_attribute(registry, a, def, "A").as_int());
  ASSERT_EQ(123, sys::get_component_attribute(registry, b, def, "A").as_int());

  ASSERT_EQ("Boo", sys::get_component_attribute(registry, a, def, "B").as_string());
  ASSERT_EQ("Coo", sys::get_component_attribute(registry, b, def, "B").as_string());

  ASSERT_EQ(1.5f, sys::get_component_attribute(registry, a, def, "C").as_float());
  ASSERT_EQ(2.0f, sys::get_component_attribute(registry, b, def, "C").as_float());

  sys::reset_component(registry, b, def);

  ASSERT_TRUE(sys::has_component(registry, a, def));
  ASSERT_TRUE(sys::has_component(registry, b, def));

  ASSERT_EQ(42, sys::get_component_attribute(registry, a, def, "A").as_int());
  ASSERT_EQ(42, sys::get_component_attribute(registry, b, def, "A").as_int());

  ASSERT_EQ("Boo", sys::get_component_attribute(registry, a, def, "B").as_string());
  ASSERT_EQ("Boo", sys::get_component_attribute(registry, b, def, "B").as_string());

  ASSERT_EQ(1.5f, sys::get_component_attribute(registry, a, def, "C").as_float());
  ASSERT_EQ(1.5f, sys::get_component_attribute(registry, b, def, "C").as_float());
}

TEST(ComponentSystem, HasComponent)
{
  auto registry = sys::make_document_registry();

  const auto a = CreateContext(registry);
  const auto b = CreateContext(registry);

  const auto def = sys::make_component_def(registry, "def");

  ASSERT_FALSE(sys::has_component(registry, a, def));
  ASSERT_FALSE(sys::has_component(registry, b, def));

  sys::add_component(registry, a, def);

  ASSERT_TRUE(sys::has_component(registry, a, def));
  ASSERT_FALSE(sys::has_component(registry, b, def));

  sys::remove_component(registry, a, def);

  ASSERT_FALSE(sys::has_component(registry, a, def));
  ASSERT_FALSE(sys::has_component(registry, b, def));
}

TEST(ComponentSystem, RemoveComponent)
{
  auto registry = sys::make_document_registry();

  const auto a = sys::make_component_def(registry, "A");
  const auto b = sys::make_component_def(registry, "B");

  const auto entity = CreateContext(registry);
  sys::add_component(registry, entity, a);
  sys::add_component(registry, entity, b);

  ASSERT_TRUE(sys::has_component(registry, entity, a));
  ASSERT_TRUE(sys::has_component(registry, entity, b));
  ASSERT_EQ(2u, sys::get_component_count(registry, entity));

  sys::remove_component(registry, entity, b);
  ASSERT_TRUE(sys::has_component(registry, entity, a));
  ASSERT_FALSE(sys::has_component(registry, entity, b));
  ASSERT_EQ(1u, sys::get_component_count(registry, entity));

  sys::remove_component(registry, entity, a);
  ASSERT_FALSE(sys::has_component(registry, entity, a));
  ASSERT_FALSE(sys::has_component(registry, entity, b));
  ASSERT_EQ(0u, sys::get_component_count(registry, entity));
}

TEST(ComponentSystem, GetComponent)
{
  auto registry = sys::make_document_registry();

  const auto a = sys::make_component_def(registry, "A");
  const auto b = sys::make_component_def(registry, "B");

  const auto entity = CreateContext(registry);
  ASSERT_THROW(sys::get_component(registry, entity, a), tactile_error);
  ASSERT_THROW(sys::get_component(registry, entity, b), tactile_error);

  sys::add_component(registry, entity, a);
  ASSERT_NO_THROW(sys::get_component(registry, entity, a));
  ASSERT_THROW(sys::get_component(registry, entity, b), tactile_error);

  const auto& component = sys::get_component(registry, entity, a);
  ASSERT_EQ(a, component.type);
}
