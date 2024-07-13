// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/meta_view_impl.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile::test {

class MetaViewImplTest : public testing::Test
{
 public:
  MetaViewImplTest()
    : mMapSpec {TileOrientation::kOrthogonal,
                MatrixExtent {5, 5},
                Int2 {32, 32}},
      mDocument {mMapSpec}
  {}

 protected:
  MapSpec mMapSpec;
  MapDocument mDocument;
};

// tactile::MetaViewImpl::MetaViewImpl
TEST_F(MetaViewImplTest, Constructor)
{
  EXPECT_ANY_THROW(MetaViewImpl(nullptr, kInvalidEntity));
}

// tactile::MetaViewImpl::get_name
TEST_F(MetaViewImplTest, GetName)
{
  const auto map_id = mDocument.get_root_entity();
  auto& registry = mDocument.get_registry();

  auto& meta = registry.get<CMeta>(map_id);
  meta.name = "foobar";

  const MetaViewImpl meta_view {&mDocument, map_id};
  EXPECT_EQ(meta_view.get_name(), "foobar");
  EXPECT_EQ(meta_view.property_count(), 0);
}

// tactile::MetaViewImpl::get_property
// tactile::MetaViewImpl::property_count
TEST_F(MetaViewImplTest, GetProperty)
{
  const auto map_id = mDocument.get_root_entity();
  auto& registry = mDocument.get_registry();

  auto& meta = registry.get<CMeta>(map_id);
  meta.properties["A"] = Attribute {1};
  meta.properties["B"] = Attribute {2};
  meta.properties["C"] = Attribute {3};

  const MetaViewImpl meta_view {&mDocument, map_id};
  EXPECT_EQ(meta_view.get_name(), "");
  EXPECT_EQ(meta_view.property_count(), 3);

  EXPECT_EQ(meta_view.get_property(0).first, "A");
  EXPECT_EQ(meta_view.get_property(0).second, Attribute {1});

  EXPECT_EQ(meta_view.get_property(1).first, "B");
  EXPECT_EQ(meta_view.get_property(1).second, Attribute {2});

  EXPECT_EQ(meta_view.get_property(2).first, "C");
  EXPECT_EQ(meta_view.get_property(2).second, Attribute {3});

  EXPECT_ANY_THROW((void) meta_view.get_property(3));
}

}  // namespace tactile::test
