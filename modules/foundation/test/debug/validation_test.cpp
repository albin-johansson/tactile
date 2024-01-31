// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/foundation/debug/validation.hpp"

#include <gtest/gtest.h>

#include "tactile/foundation/debug/generic_error.hpp"

using namespace tactile;

// NOLINTBEGIN(*-avoid-goto)
// NOLINTBEGIN(*-type-traits)
// NOLINTBEGIN(*-function-cognitive-complexity)
// NOLINTBEGIN(*-use-trailing-return-type)

/// \tests tactile::require_not_null
TEST(Validation, RequireNotNull)
{
  const auto var = 42;
  EXPECT_EQ(require_not_null(&var), &var);
  EXPECT_THROW((void) require_not_null(nullptr), Exception);
}

/// \tests tactile::require_ok
TEST(Validation, RequireOk)
{
  const auto not_ok = unexpected(make_error(GenericError::kUnknown));
  EXPECT_NO_THROW(require_ok(kOK));
  EXPECT_THROW(require_ok(not_ok), Exception);
}

// NOLINTEND(*-use-trailing-return-type)
// NOLINTEND(*-function-cognitive-complexity)
// NOLINTEND(*-type-traits)
// NOLINTEND(*-avoid-goto)
