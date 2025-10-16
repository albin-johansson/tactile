// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include <gtest/gtest.h>

#include "tactile/editor/util/logger_context.hpp"

namespace tactile {
namespace {

// NOLINTNEXTLINE(*-avoid-c-arrays)
auto run_tests(int argc, char* argv[]) -> int
{
  const editor::LoggerContext logger_context {};
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

}  // namespace
}  // namespace tactile

auto main(const int argc, char* argv[]) -> int
{
  return tactile::run_tests(argc, argv);
}
