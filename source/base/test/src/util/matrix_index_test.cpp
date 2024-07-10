// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/util/matrix_index.hpp"

#include <format>  // format

#include <gtest/gtest.h>

namespace tactile {

TEST(MatrixIndex, Format)
{
  EXPECT_EQ(std::format("{}", MatrixIndex {1, 2}), "(1, 2)");
}

}  // namespace tactile
