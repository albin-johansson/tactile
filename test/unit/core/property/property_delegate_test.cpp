#include "property_delegate.hpp"

#include <gtest/gtest.h>

#include "tactile_qstring.hpp"

using namespace tactile;

TEST(PropertyDelegate, AddProperty)
{
  core::property_delegate delegate;
  EXPECT_EQ(0, delegate.property_count());

  const auto name = TACTILE_QSTRING(u"foo");
  delegate.add_property(name, core::property::integer);
  EXPECT_EQ(1, delegate.property_count());

  const auto& property = delegate.get_property(name);
  ASSERT_TRUE(property.is_integer());
  EXPECT_EQ(0, property.as_integer());
}

TEST(PropertyDelegate, RemoveProperty)
{
  core::property_delegate delegate;
  const auto name = TACTILE_QSTRING(u"foo");

  delegate.add_property(name, core::property::integer);
  EXPECT_EQ(1, delegate.property_count());
  EXPECT_NO_THROW(delegate.get_property(name));

  delegate.remove_property(name);
  EXPECT_EQ(0, delegate.property_count());
  EXPECT_ANY_THROW(delegate.get_property(name));
}

TEST(PropertyDelegate, RenameProperty)
{
  core::property_delegate delegate;

  const auto oldName = TACTILE_QSTRING(u"foo");
  delegate.add_property(oldName, core::property::boolean);
  EXPECT_NO_THROW(delegate.get_property(oldName));

  const auto newName = TACTILE_QSTRING(u"bar");
  EXPECT_NO_THROW(delegate.rename_property(oldName, newName));
  EXPECT_ANY_THROW(delegate.get_property(oldName));

  const auto& property = delegate.get_property(newName);
  EXPECT_TRUE(property.is_boolean());
}

TEST(PropertyDelegate, SetProperty)
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");

  delegate.add_property(name, core::property::integer);
  EXPECT_EQ(0, delegate.get_property(name).as_integer());

  delegate.set_property(name, 123);
  EXPECT_EQ(123, delegate.get_property(name).as_integer());
}

TEST(PropertyDelegate, GetProperty)
{
  core::property_delegate delegate;

  const auto name = TACTILE_QSTRING(u"foo");
  EXPECT_ANY_THROW(delegate.get_property(name));

  delegate.add_property(name, core::property::string);
  EXPECT_NO_THROW(delegate.get_property(name));

  const auto& cDelegate = delegate;
  EXPECT_NO_THROW(cDelegate.get_property(name));
}

TEST(PropertyDelegate, PropertyCount)
{
  core::property_delegate delegate;
  EXPECT_EQ(0, delegate.property_count());

  delegate.add_property(TACTILE_QSTRING(u"foo"), core::property::string);
  EXPECT_EQ(1, delegate.property_count());

  delegate.add_property(TACTILE_QSTRING(u"bar"), core::property::integer);
  EXPECT_EQ(2, delegate.property_count());

  delegate.remove_property(TACTILE_QSTRING(u"bar"));
  EXPECT_EQ(1, delegate.property_count());

  delegate.remove_property(TACTILE_QSTRING(u"foo"));
  EXPECT_EQ(0, delegate.property_count());
}
