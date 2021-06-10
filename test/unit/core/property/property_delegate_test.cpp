#include "property_delegate.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(PropertyDelegate, AddProperty)
{
  core::property_delegate delegate;
  EXPECT_EQ(0, delegate.PropertyCount());

  const auto name = TACTILE_QSTRING(u"foo");
  delegate.AddProperty(name, core::property_type::integer);
  EXPECT_EQ(1, delegate.PropertyCount());

  const auto& property = delegate.GetProperty(name);
  ASSERT_TRUE(property.is_integer());
  EXPECT_EQ(0, property.as_integer());
}

TEST(PropertyDelegate, RemoveProperty)
{
  core::property_delegate delegate;
  const auto name = TACTILE_QSTRING(u"foo");

  delegate.AddProperty(name, core::property_type::integer);
  EXPECT_EQ(1, delegate.PropertyCount());
  EXPECT_NO_THROW(delegate.GetProperty(name));

  delegate.RemoveProperty(name);
  EXPECT_EQ(0, delegate.PropertyCount());
  EXPECT_ANY_THROW(delegate.GetProperty(name));
}

TEST(PropertyDelegate, RenameProperty)
{
  core::property_delegate delegate;

  const auto oldName = TACTILE_QSTRING(u"foo");
  delegate.AddProperty(oldName, core::property_type::boolean);
  EXPECT_NO_THROW(delegate.GetProperty(oldName));

  const auto newName = TACTILE_QSTRING(u"bar");
  EXPECT_NO_THROW(delegate.RenameProperty(oldName, newName));
  EXPECT_ANY_THROW(delegate.GetProperty(oldName));

  const auto& property = delegate.GetProperty(newName);
  EXPECT_TRUE(property.is_boolean());
}

TEST(PropertyDelegate, SetProperty)
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");

  delegate.AddProperty(name, core::property_type::integer);
  EXPECT_EQ(0, delegate.GetProperty(name).as_integer());

  delegate.SetProperty(name, 123);
  EXPECT_EQ(123, delegate.GetProperty(name).as_integer());
}

TEST(PropertyDelegate, GetProperty)
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");
  EXPECT_ANY_THROW(delegate.GetProperty(name));

  delegate.AddProperty(name, core::property_type::string);
  EXPECT_NO_THROW(delegate.GetProperty(name));

  const auto& cDelegate = delegate;
  EXPECT_NO_THROW(cDelegate.GetProperty(name));
}

TEST(PropertyDelegate, PropertyCount)
{
  core::property_delegate delegate;
  EXPECT_EQ(0, delegate.PropertyCount());

  delegate.AddProperty(TACTILE_QSTRING(u"foo"), core::property_type::string);
  EXPECT_EQ(1, delegate.PropertyCount());

  delegate.AddProperty(TACTILE_QSTRING(u"bar"), core::property_type::integer);
  EXPECT_EQ(2, delegate.PropertyCount());

  delegate.RemoveProperty(TACTILE_QSTRING(u"bar"));
  EXPECT_EQ(1, delegate.PropertyCount());

  delegate.RemoveProperty(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(0, delegate.PropertyCount());
}
