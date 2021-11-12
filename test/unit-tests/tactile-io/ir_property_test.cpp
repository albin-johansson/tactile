#include <filesystem>  // path

#include <tactile_io.hpp>

#include <gtest/gtest.h>

using namespace Tactile;

TEST(IrProperty, AddProperty)
{
  auto map = IO::CreateMap();
  ASSERT_EQ(0u, IO::GetPropertyCount(*map));

  auto& property = IO::AddProperty(*map);
  ASSERT_EQ(1u, IO::GetPropertyCount(*map));
  ASSERT_EQ(PropertyType::String, IO::GetType(property));
  ASSERT_TRUE(IO::IsString(property));
  ASSERT_STREQ("", IO::GetString(property));
}

TEST(IrProperty, SetName)
{
  auto map = IO::CreateMap();
  auto& property = IO::AddProperty(*map);
  ASSERT_STREQ("", IO::GetName(property));

  IO::SetName(property, "foobar");
  ASSERT_STREQ("foobar", IO::GetName(property));
}

TEST(IrProperty, Assignment)
{
  auto map = IO::CreateMap();
  auto& property = IO::AddProperty(*map);
  ASSERT_EQ(PropertyType::String, IO::GetType(property));

  IO::AssignString(property, "foo");
  ASSERT_TRUE(IO::IsString(property));
  ASSERT_FALSE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsObject(property));
  ASSERT_FALSE(IO::IsFile(property));
  ASSERT_STREQ("foo", IO::GetString(property));

  IO::AssignInt(property, 42);
  ASSERT_TRUE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsString(property));
  ASSERT_FALSE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsObject(property));
  ASSERT_FALSE(IO::IsFile(property));
  ASSERT_EQ(42, IO::GetInt(property));

  IO::AssignFloat(property, 7.5f);
  ASSERT_TRUE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsString(property));
  ASSERT_FALSE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsObject(property));
  ASSERT_FALSE(IO::IsFile(property));
  ASSERT_FLOAT_EQ(7.5f, IO::GetFloat(property));

  IO::AssignBool(property, true);
  ASSERT_TRUE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsString(property));
  ASSERT_FALSE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsObject(property));
  ASSERT_FALSE(IO::IsFile(property));
  ASSERT_TRUE(IO::GetBool(property));

  IO::AssignColor(property, {.red = 0xF1, .green = 0x39, .blue = 0xE7, .alpha = 0xCB});
  ASSERT_TRUE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsString(property));
  ASSERT_FALSE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsObject(property));
  ASSERT_FALSE(IO::IsFile(property));
  ASSERT_EQ(0xF1, IO::GetColor(property).red);
  ASSERT_EQ(0x39, IO::GetColor(property).green);
  ASSERT_EQ(0xE7, IO::GetColor(property).blue);
  ASSERT_EQ(0xCB, IO::GetColor(property).alpha);

  IO::AssignObject(property, 123);
  ASSERT_TRUE(IO::IsObject(property));
  ASSERT_FALSE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsString(property));
  ASSERT_FALSE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsFile(property));
  ASSERT_EQ(123, IO::GetObject(property));

  const std::filesystem::path file = "foo/bar.yaml";
  IO::AssignFile(property, file.c_str());
  ASSERT_TRUE(IO::IsFile(property));
  ASSERT_FALSE(IO::IsInt(property));
  ASSERT_FALSE(IO::IsFloat(property));
  ASSERT_FALSE(IO::IsString(property));
  ASSERT_FALSE(IO::IsBool(property));
  ASSERT_FALSE(IO::IsColor(property));
  ASSERT_FALSE(IO::IsObject(property));
  ASSERT_EQ(file, IO::GetFile(property));
}