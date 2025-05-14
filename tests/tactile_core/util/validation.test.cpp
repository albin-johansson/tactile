// Copyright (C) 2025 Albin Johansson

#include <gtest/gtest.h>

import std;
import tactile.core.util;

namespace tactile::tests {
namespace {

class ValidationTest : public testing::Test
{};

TEST_F(ValidationTest, RequireNotNull)
{
  const auto foo = 42;
  EXPECT_EQ(require_not_null(&foo), &foo);

  EXPECT_THROW((void) require_not_null(nullptr), std::invalid_argument);
}

}  // namespace
}  // namespace tactile::tests
