#include "core/properties/property_delegate.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(PropertyDelegate, Defaults)
{
  const PropertyDelegate delegate;
  ASSERT_EQ(0, delegate.GetPropertyCount());
  ASSERT_TRUE(delegate.GetProperties().empty());
  ASSERT_TRUE(delegate.GetName().empty());
}

TEST(PropertyDelegate, AddProperty)
{
  PropertyDelegate delegate;
  ASSERT_FALSE(delegate.HasProperty("foo"));
  ASSERT_EQ(0, delegate.GetPropertyCount());

  delegate.AddProperty("foo", PropertyType::Integer);
  ASSERT_TRUE(delegate.HasProperty("foo"));
  ASSERT_EQ(1, delegate.GetPropertyCount());

  const auto& property = delegate.GetProperty("foo");
  ASSERT_TRUE(property.IsInteger());
  ASSERT_EQ(0, property.AsInteger());
}

TEST(PropertyDelegate, RemoveProperty)
{
  PropertyDelegate delegate;

  delegate.AddProperty("foo", PropertyType::Integer);
  ASSERT_TRUE(delegate.HasProperty("foo"));
  ASSERT_EQ(1, delegate.GetPropertyCount());

  delegate.RemoveProperty("foo");
  ASSERT_FALSE(delegate.HasProperty("foo"));
  ASSERT_EQ(0, delegate.GetPropertyCount());
}

TEST(PropertyDelegate, RenameProperty)
{
  PropertyDelegate delegate;

  const auto oldName = "foo";
  const auto newName = "bar";

  delegate.AddProperty(oldName, PropertyType::Boolean);
  ASSERT_TRUE(delegate.HasProperty(oldName));
  ASSERT_FALSE(delegate.HasProperty(newName));

  ASSERT_NO_THROW(delegate.RenameProperty(oldName, newName));
  ASSERT_FALSE(delegate.HasProperty(oldName));
  ASSERT_TRUE(delegate.HasProperty(newName));

  const auto& property = delegate.GetProperty(newName);
  ASSERT_TRUE(property.IsBoolean());
}

TEST(PropertyDelegate, SetProperty)
{
  PropertyDelegate delegate;

  const auto name = "foo";

  delegate.AddProperty(name, PropertyType::Integer);
  ASSERT_EQ(0, delegate.GetProperty(name).AsInteger());

  delegate.SetProperty(name, 123);
  ASSERT_EQ(123, delegate.GetProperty(name).AsInteger());
}

TEST(PropertyDelegate, GetProperty)
{
  PropertyDelegate delegate;

  const auto name = "foo";
  ASSERT_ANY_THROW(delegate.GetProperty(name));

  delegate.AddProperty(name, PropertyType::String);
  ASSERT_NO_THROW(delegate.GetProperty(name));
}

TEST(PropertyDelegate, GetPropertyCount)
{
  PropertyDelegate delegate;
  ASSERT_EQ(0, delegate.GetPropertyCount());

  delegate.AddProperty("foo", PropertyType::String);
  ASSERT_EQ(1, delegate.GetPropertyCount());

  delegate.AddProperty("bar", PropertyType::Integer);
  ASSERT_EQ(2, delegate.GetPropertyCount());

  delegate.RemoveProperty("bar");
  ASSERT_EQ(1, delegate.GetPropertyCount());

  delegate.RemoveProperty("foo");
  ASSERT_EQ(0, delegate.GetPropertyCount());
}
