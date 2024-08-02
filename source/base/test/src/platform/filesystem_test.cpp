// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/base/platform/filesystem.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

// tactile::normalize_path_separators
TEST(Filesystem, NormalizePathSeparators)
{
  const auto path = Path {"f"} / "o" / "o" / "b" / "a" / "r";
  EXPECT_EQ(normalize_path_separators(path), "f/o/o/b/a/r");
}

}  // namespace tactile::test
