// Copyright (C) 2025 Albin Johansson
// This software is provided under the terms of the MIT License.

#include "core/layer/object_layer.hpp"

#include <gtest/gtest.h>

namespace tactile::test {

TEST(ObjectLayer, Defaults)
{
  const ObjectLayer layer;
  ASSERT_EQ(0, layer.object_count());
}

}  // namespace tactile::test
