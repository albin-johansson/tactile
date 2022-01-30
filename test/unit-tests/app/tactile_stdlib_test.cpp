#include <tactile_stdlib.hpp>

#include <gtest/gtest.h>

using namespace tactile;

TEST(TactileStdLib, ToMatrixCoords)
{
  ASSERT_EQ(0, ToMatrixCoords(0, 3).first);
  ASSERT_EQ(0, ToMatrixCoords(0, 3).second);

  ASSERT_EQ(0, ToMatrixCoords(1, 3).first);
  ASSERT_EQ(1, ToMatrixCoords(1, 3).second);

  ASSERT_EQ(0, ToMatrixCoords(2, 3).first);
  ASSERT_EQ(2, ToMatrixCoords(2, 3).second);

  ASSERT_EQ(1, ToMatrixCoords(3, 3).first);
  ASSERT_EQ(0, ToMatrixCoords(3, 3).second);

  ASSERT_EQ(1, ToMatrixCoords(4, 3).first);
  ASSERT_EQ(1, ToMatrixCoords(4, 3).second);

  ASSERT_EQ(1, ToMatrixCoords(5, 3).first);
  ASSERT_EQ(2, ToMatrixCoords(5, 3).second);
}

TEST(TactileStdLib, MakeTileRow)
{
  const auto row = MakeTileRow(10);
  ASSERT_EQ(10, row.capacity());
  ASSERT_EQ(10, row.size());
}

TEST(TactileStdLib, MakeTileMatrix)
{
  const auto matrix = MakeTileMatrix(5, 7);
  ASSERT_EQ(5, matrix.capacity());
  ASSERT_EQ(5, matrix.size());
  ASSERT_EQ(7, matrix.at(0).capacity());
  ASSERT_EQ(7, matrix.at(0).size());
}

TEST(TactileStdLib, IntegerFromString)
{
  ASSERT_FALSE(FromString<int>("ABC"));

  ASSERT_EQ(-42, FromString<int>("-42"));
  ASSERT_EQ(123, FromString<int16>("123"));
  ASSERT_EQ(745, FromString<int64>("745"));

  ASSERT_FALSE(FromString<int>("F1"));
  ASSERT_EQ(0xF1, FromString<int>("F1", 16));

  ASSERT_EQ(4, FromString<int>("4298", 1u, 10));
  ASSERT_EQ(0x12, FromString<int>("12AC", 2u, 16));

  // Robustness against invalid length
  ASSERT_EQ(1234, FromString<int>("1234"));
  ASSERT_EQ(1234, FromString<int>("1234", 4u, 10));
  ASSERT_EQ(1234, FromString<int>("1234", 5u, 10));
  ASSERT_EQ(1234, FromString<int>("1234", 6u, 10));
}

TEST(TactileStdLib, FloatFromString)
{
  ASSERT_FALSE(FromString<float>("foo"));

  ASSERT_EQ(12.3f, FromString<float>("12.3"));
  ASSERT_EQ(42.0f, FromString<float>("42"));

  ASSERT_EQ(78.5, FromString<double>("78.5"));
  ASSERT_EQ(98.0, FromString<double>("98"));
}

TEST(TactileStdLib, ConvertToForwardSlashes)
{
  const std::filesystem::path source = R"(C:\foo\bar\abc.yaml)";
  ASSERT_EQ("C:/foo/bar/abc.yaml", ConvertToForwardSlashes(source));
}