#include "core/systems/context_system.hpp"

#include <gtest/gtest.h>

#include "core/systems/component_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

TEST(ContextSystem, ResetNextContextId)
{
  sys::reset_next_context_id();
  ASSERT_EQ(1, sys::next_context_id());
}

TEST(ContextSystem, CurrentContext)
{
  auto registry = sys::make_document_registry();
  ASSERT_NO_THROW({ auto& root [[maybe_unused]] = sys::current_context(registry); });
  ASSERT_EQ(1, sys::current_context_id(registry));
}

TEST(ContextSystem, AddAttributeContext)
{
  auto registry = sys::make_document_registry();

  const auto a = registry.create();
  const auto b = registry.create();

  const auto& first = sys::add_attribute_context(registry, a);
  ASSERT_GT(first.id, 0);
  ASSERT_TRUE(first.name.empty());
  ASSERT_TRUE(first.properties.empty());
  ASSERT_TRUE(first.components.empty());

  const auto& second = sys::add_attribute_context(registry, b);
  ASSERT_GT(second.id, first.id);
  ASSERT_TRUE(second.name.empty());
  ASSERT_TRUE(second.properties.empty());
  ASSERT_TRUE(second.components.empty());
}

TEST(ContextSystem, CopyAttributeContext)
{
  using namespace std::string_literals;
  auto registry = sys::make_document_registry();

  const auto componentId = sys::make_component_def(registry, "Component");
  sys::make_component_attribute(registry, componentId, "number", 1'337);
  sys::make_component_attribute(registry, componentId, "str", "hello"s);

  const auto entity = registry.create();
  auto& context = sys::add_attribute_context(registry, entity);
  context.name = "Foo";

  sys::add_component(registry, context.id, componentId);

  sys::add_property(registry, context, "A", 123);
  sys::add_property(registry, context, "B", "abc"s);

  const auto snapshot = sys::copy_attribute_context(registry, entity);
  ASSERT_EQ(context.id, snapshot.id);
  ASSERT_EQ(context.name, snapshot.name);

  ASSERT_EQ(2u, snapshot.properties.size());
  ASSERT_EQ(1u, snapshot.components.size());

  ASSERT_EQ(123, snapshot.properties.at("A").as_int());
  ASSERT_EQ("abc", snapshot.properties.at("B").as_string());

  ASSERT_EQ(1'337, snapshot.components.at("Component").attributes.at("number"));
  ASSERT_EQ("hello", snapshot.components.at("Component").attributes.at("str"));
}

TEST(ContextSystem, RestoreAttributeContext)
{
  auto registry = sys::make_document_registry();

  const auto oldEntity = registry.create();
  const auto next = sys::next_context_id();

  {
    auto& context = sys::add_attribute_context(registry, oldEntity);
    context.name = "ABC";

    ASSERT_EQ(next, context.id);

    sys::add_property(registry, context, "A", 12);
    sys::add_property(registry, context, "B", 3.14f);

    ASSERT_TRUE(sys::has_property_with_name(registry, context, "A"));
    ASSERT_TRUE(sys::has_property_with_name(registry, context, "B"));
  }

  const auto snapshot = sys::copy_attribute_context(registry, oldEntity);

  {
    const auto newEntity = registry.create();
    sys::restore_attribute_context(registry, newEntity, snapshot);

    const auto& context = registry.get<comp::attribute_context>(newEntity);
    ASSERT_EQ(next, context.id);
    ASSERT_EQ("ABC", context.name);

    ASSERT_TRUE(sys::has_property_with_name(registry, context, "A"));
    ASSERT_TRUE(sys::has_property_with_name(registry, context, "B"));

    ASSERT_EQ(12, sys::get_property(registry, context, "A").value.as_int());
    ASSERT_EQ(3.14f, sys::get_property(registry, context, "B").value.as_float());
  }
}