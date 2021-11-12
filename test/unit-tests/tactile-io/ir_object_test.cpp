#include <tactile_io.hpp>

#include <gtest/gtest.h>

using namespace Tactile;

TEST(IrObject, Defaults)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);

  auto& objectLayer = IO::MarkAsObjectLayer(layer);
  ASSERT_EQ(0u, IO::GetObjectCount(objectLayer));

  auto& object = IO::AddObject(objectLayer);
  ASSERT_EQ(1u, IO::GetObjectCount(objectLayer));

  ASSERT_EQ(0, IO::GetId(object));
  ASSERT_EQ(ObjectType::Rectangle, IO::GetType(object));

  ASSERT_STREQ("", IO::GetName(object));
  ASSERT_STREQ("", IO::GetTag(object));

  ASSERT_FLOAT_EQ(0, IO::GetX(object));
  ASSERT_FLOAT_EQ(0, IO::GetY(object));
  ASSERT_FLOAT_EQ(0, IO::GetWidth(object));
  ASSERT_FLOAT_EQ(0, IO::GetHeight(object));

  ASSERT_TRUE(IO::IsVisible(object));
  ASSERT_EQ(0u, IO::GetPropertyCount(object));
}

TEST(IrObject, Usage)
{
  auto map = IO::CreateMap();
  auto& layer = IO::AddLayer(*map);

  auto& objectLayer = IO::MarkAsObjectLayer(layer);
  auto& object = IO::AddObject(objectLayer);

  IO::SetX(object, 42.5f);
  ASSERT_FLOAT_EQ(42.5f, IO::GetX(object));

  IO::SetY(object, 84.7f);
  ASSERT_FLOAT_EQ(84.7f, IO::GetY(object));

  IO::SetWidth(object, 472.1f);
  ASSERT_FLOAT_EQ(472.1f, IO::GetWidth(object));

  IO::SetHeight(object, 283.8f);
  ASSERT_FLOAT_EQ(283.8f, IO::GetHeight(object));

  IO::SetType(object, ObjectType::Ellipse);
  ASSERT_EQ(ObjectType::Ellipse, IO::GetType(object));

  IO::SetTag(object, "foo");
  ASSERT_STREQ("foo", IO::GetTag(object));

  IO::SetVisible(object, false);
  ASSERT_FALSE(IO::IsVisible(object));
}