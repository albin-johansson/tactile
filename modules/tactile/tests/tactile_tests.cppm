// Copyright (C) 2025 Albin Johansson (GNU General Public License v3.0)

module;

#include <gtest/gtest.h>

export module tactile.tests;

auto main(int argc, char* argv[]) -> int
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
