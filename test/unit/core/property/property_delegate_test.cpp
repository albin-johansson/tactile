#include "property_delegate.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(PropertyDelegate, AddProperty)
{
  core::PropertyDelegate delegate;
  EXPECT_EQ(0, delegate.PropertyCount());

  const auto name = TACTILE_QSTRING(u"foo");
  delegate.AddProperty(name, core::PropertyType::Integer);
  EXPECT_EQ(1, delegate.PropertyCount());

  const auto& property = delegate.GetProperty(name);
  ASSERT_TRUE(property.IsInteger());
  EXPECT_EQ(0, property.AsInteger());
}

TEST(PropertyDelegate, RemoveProperty)
{
  core::PropertyDelegate delegate;
  const auto name = TACTILE_QSTRING(u"foo");

  delegate.AddProperty(name, core::PropertyType::Integer);
  EXPECT_EQ(1, delegate.PropertyCount());
  EXPECT_NO_THROW(delegate.GetProperty(name));

  delegate.RemoveProperty(name);
  EXPECT_EQ(0, delegate.PropertyCount());
  EXPECT_ANY_THROW(delegate.GetProperty(name));
}

TEST(PropertyDelegate, RenameProperty)
{
  core::PropertyDelegate delegate;

  const auto oldName = TACTILE_QSTRING(u"foo");
  delegate.AddProperty(oldName, core::PropertyType::Boolean);
  EXPECT_NO_THROW(delegate.GetProperty(oldName));

  const auto newName = TACTILE_QSTRING(u"bar");
  EXPECT_NO_THROW(delegate.RenameProperty(oldName, newName));
  EXPECT_ANY_THROW(delegate.GetProperty(oldName));

  const auto& property = delegate.GetProperty(newName);
  EXPECT_TRUE(property.IsBoolean());
}

TEST(PropertyDelegate, SetProperty)
{
  core::PropertyDelegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");

  delegate.AddProperty(name, core::PropertyType::Integer);
  EXPECT_EQ(0, delegate.GetProperty(name).AsInteger());

  delegate.SetProperty(name, 123);
  EXPECT_EQ(123, delegate.GetProperty(name).AsInteger());
}

TEST(PropertyDelegate, GetProperty)
{
  core::PropertyDelegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");
  EXPECT_ANY_THROW(delegate.GetProperty(name));

  delegate.AddProperty(name, core::PropertyType::String);
  EXPECT_NO_THROW(delegate.GetProperty(name));

  const auto& cDelegate = delegate;
  EXPECT_NO_THROW(cDelegate.GetProperty(name));
}

TEST(PropertyDelegate, PropertyCount)
{
  core::PropertyDelegate delegate;
  EXPECT_EQ(0, delegate.PropertyCount());

  delegate.AddProperty(TACTILE_QSTRING(u"foo"), core::PropertyType::String);
  EXPECT_EQ(1, delegate.PropertyCount());

  delegate.AddProperty(TACTILE_QSTRING(u"bar"), core::PropertyType::Integer);
  EXPECT_EQ(2, delegate.PropertyCount());

  delegate.RemoveProperty(TACTILE_QSTRING(u"bar"));
  EXPECT_EQ(1, delegate.PropertyCount());

  delegate.RemoveProperty(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(0, delegate.PropertyCount());
}
