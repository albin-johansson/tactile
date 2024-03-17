// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/util/scope_guard.hpp"

#include <gtest/gtest.h>

namespace tactile {

/** \trace tactile::ScopeGuard::ScopeGuard */
TEST(ScopeGuard, ExampleUsage)
{
  int i = 0;

  {
    const ScopeGuard guard {[&] { i = 10; }};
    i = 20;
  }

  EXPECT_EQ(i, 10);
}

}  // namespace tactile
