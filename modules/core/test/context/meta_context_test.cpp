// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include <gtest/gtest.h>

#include "tactile/core/map/layer/group_layer.hpp"
#include "tactile/core/map/layer/object_layer.hpp"
#include "tactile/core/map/layer/tile_layer.hpp"

using namespace tactile;

template <typename T>
class IMetaContextTest : public testing::Test {};

using MetaContextTypes = testing::Types<TileLayer, ObjectLayer, GroupLayer>;

TYPED_TEST_SUITE(IMetaContextTest, MetaContextTypes);

/// \tests tactile::IMetaContext::get_meta
TYPED_TEST(IMetaContextTest, GetMeta)
{
  TypeParam context {};
  const auto& const_context = context;

  context.get_meta().set_name("foo");

  EXPECT_EQ(context.get_meta().get_name(), "foo");
  EXPECT_EQ(const_context.get_meta().get_name(), "foo");
}
