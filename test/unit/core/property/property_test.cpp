#include "property.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(Property, Defaults)
{
  const core::Property property;
  EXPECT_FALSE(property.HasValue());
  EXPECT_FALSE(property.Type());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsFile());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, IntProperty)
{
  const core::Property property{123};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsInteger());
  EXPECT_TRUE(property.TryAsInteger());
  EXPECT_EQ(123, property.AsInteger());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsFile());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, FloatProperty)
{
  const core::Property property{12.3};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsFloating());
  EXPECT_TRUE(property.TryAsFloating());
  EXPECT_EQ(12.3, property.AsFloating());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsFile());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, StringProperty)
{
  const core::Property property{QStringLiteral(u"foo")};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsString());
  EXPECT_TRUE(property.TryAsString());
  EXPECT_EQ(QStringLiteral(u"foo"), property.AsString());

  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsFile());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, BoolProperty)
{
  const core::Property property{false};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsBoolean());
  EXPECT_TRUE(property.TryAsBoolean());
  EXPECT_FALSE(property.AsBoolean());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsFile());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, FileProperty)
{
  const QFileInfo file{QStringLiteral(u"resources/foo.txt")};
  const core::Property property{file};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsFile());
  EXPECT_TRUE(property.TryAsFile());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, ObjectProperty)
{
  const core::Property property{core::object_ref{7}};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsObject());
  EXPECT_TRUE(property.TryAsObject());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsColor());
  EXPECT_FALSE(property.IsFile());
}

TEST(Property, ColorProperty)
{
  const QColor color{Qt::red};
  const core::Property property{color};
  ASSERT_TRUE(property.HasValue());
  ASSERT_TRUE(property.IsColor());
  EXPECT_TRUE(property.TryAsColor());

  EXPECT_FALSE(property.IsString());
  EXPECT_FALSE(property.IsInteger());
  EXPECT_FALSE(property.IsFloating());
  EXPECT_FALSE(property.IsBoolean());
  EXPECT_FALSE(property.IsFile());
  EXPECT_FALSE(property.IsObject());
}

TEST(Property, Reset)
{
  core::Property property;
  ASSERT_FALSE(property.HasValue());

  property.SetValue(123);
  EXPECT_TRUE(property.HasValue());

  property.Reset();
  EXPECT_FALSE(property.HasValue());

  EXPECT_NO_THROW(property.Reset());
  EXPECT_NO_THROW(property.Reset());
}

TEST(Property, SetValue)
{
  core::Property property;

  property.SetValue(10);
  ASSERT_TRUE(property.IsInteger());
  EXPECT_EQ(10, property.AsInteger());

  property.SetValue(93.2);
  ASSERT_TRUE(property.IsFloating());
  EXPECT_EQ(93.2, property.AsFloating());

  property.SetValue(true);
  ASSERT_TRUE(property.IsBoolean());
  EXPECT_TRUE(property.AsBoolean());

  property.SetValue(QStringLiteral(u"foo"));
  ASSERT_TRUE(property.IsString());
  EXPECT_EQ(QStringLiteral(u"foo"), property.AsString());
}

TEST(Property, HasValue)
{
  core::Property property;
  EXPECT_FALSE(property.HasValue());

  property.SetValue(924);
  EXPECT_TRUE(property.HasValue());
}

TEST(Property, As)
{
  const core::Property property{123};

  EXPECT_NO_THROW(property.AsInteger());
  EXPECT_ANY_THROW(property.AsFloating());
  EXPECT_ANY_THROW(property.AsString());
  EXPECT_ANY_THROW(property.AsBoolean());
  EXPECT_ANY_THROW(property.AsObject());
  EXPECT_ANY_THROW(property.AsFile());
  EXPECT_ANY_THROW(property.AsColor());
}

TEST(Property, TryAs)
{
  core::Property property;

  EXPECT_FALSE(property.TryAsInteger());
  EXPECT_FALSE(property.TryAsFloating());
  EXPECT_FALSE(property.TryAsString());
  EXPECT_FALSE(property.TryAsBoolean());

  property.SetValue(123);
  EXPECT_TRUE(property.TryAsInteger());
  EXPECT_FALSE(property.TryAsFloating());
  EXPECT_FALSE(property.TryAsString());
  EXPECT_FALSE(property.TryAsBoolean());
}

TEST(Property, Type)
{
  core::Property property;
  EXPECT_FALSE(property.Type());

  property.SetValue(123);
  EXPECT_EQ(core::PropertyType::Integer, property.Type());

  property.SetValue(12.3);
  EXPECT_EQ(core::PropertyType::Floating, property.Type());

  property.SetValue(QStringLiteral(u"foo"));
  EXPECT_EQ(core::PropertyType::String, property.Type());

  property.Reset();
  EXPECT_FALSE(property.Type());
}
