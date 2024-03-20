// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/debug/validation.hpp"

#include <gtest/gtest.h>

namespace tactile {

/** \trace tactile::require_not_null */
TEST(Validation, RequireNotNullWithValidPointer)
{
  const auto value = 42;
  EXPECT_EQ(require_not_null(&value), &value);
}

/** \trace tactile::require_not_null */
TEST(Validation, RequireNotNullWithNullPointer)
{
  EXPECT_THROW((void) require_not_null(nullptr), Exception);
}

}  // namespace tactile
