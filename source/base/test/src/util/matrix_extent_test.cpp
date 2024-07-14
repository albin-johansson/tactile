// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/util/matrix_extent.hpp"

#include <format>  // format

#include <gtest/gtest.h>

namespace tactile {

TEST(MatrixExtent, Format)
{
  const MatrixExtent extent {12, 34};
  EXPECT_EQ(std::format("{}", extent), "(12, 34)");
}

}  // namespace tactile
