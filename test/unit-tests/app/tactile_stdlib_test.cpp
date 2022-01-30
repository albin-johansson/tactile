#include "tactile_stdlib.hpp"

#include <gtest/gtest.h>

using namespace tactile;

TEST(TactileStdLib, ToMatrixCoords)
{
  ASSERT_EQ(0, to_matrix_coords(0, 3).first);
  ASSERT_EQ(0, to_matrix_coords(0, 3).second);

  ASSERT_EQ(0, to_matrix_coords(1, 3).first);
  ASSERT_EQ(1, to_matrix_coords(1, 3).second);

  ASSERT_EQ(0, to_matrix_coords(2, 3).first);
  ASSERT_EQ(2, to_matrix_coords(2, 3).second);

  ASSERT_EQ(1, to_matrix_coords(3, 3).first);
  ASSERT_EQ(0, to_matrix_coords(3, 3).second);

  ASSERT_EQ(1, to_matrix_coords(4, 3).first);
  ASSERT_EQ(1, to_matrix_coords(4, 3).second);

  ASSERT_EQ(1, to_matrix_coords(5, 3).first);
  ASSERT_EQ(2, to_matrix_coords(5, 3).second);
}

TEST(TactileStdLib, ConvertToForwardSlashes)
{
  const std::filesystem::path source = R"(C:\foo\bar\abc.yaml)";
  ASSERT_EQ("C:/foo/bar/abc.yaml", convert_to_forward_slashes(source));
}