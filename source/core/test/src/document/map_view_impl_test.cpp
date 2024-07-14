// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/document/map_view_impl.hpp"

#include <gtest/gtest.h>

#include "tactile/core/document/map_document.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/group_layer.hpp"
#include "tactile/core/layer/object_layer.hpp"
#include "tactile/core/map/map.hpp"
#include "tactile/core/map/map_spec.hpp"
#include "test/document_testing.hpp"

namespace tactile::test {

class MapViewImplTest : public testing::Test
{
 public:
  MapViewImplTest()
    : mMapSpec {kOrthogonalMapSpec},
      mDocument {mMapSpec}
  {}

 protected:
  MapSpec mMapSpec;
  MapDocument mDocument;
};

// tactile::MapViewImpl::get_tile_size
// tactile::MapViewImpl::get_extent
// tactile::MapViewImpl::get_next_layer_id
// tactile::MapViewImpl::get_next_object_id
// tactile::MapViewImpl::layer_count
// tactile::MapViewImpl::tileset_count
TEST_F(MapViewImplTest, Getters)
{
  const auto map_id = mDocument.get_root_entity();
  auto& registry = mDocument.get_registry();

  auto& map = registry.get<CMap>(map_id);
  map.attached_tilesets.push_back(registry.make_entity());

  auto& id_cache = registry.get<CMapIdCache>(map_id);
  id_cache.next_layer_id = LayerID {7};
  id_cache.next_object_id = ObjectID {42};

  auto& root_layer = registry.get<CGroupLayer>(map.root_layer);
  root_layer.layers.push_back(make_object_layer(registry));
  root_layer.layers.push_back(make_object_layer(registry));
  root_layer.layers.push_back(make_object_layer(registry));

  const MapViewImpl map_view {&mDocument};

  EXPECT_EQ(map_view.get_tile_size(), map.tile_size);
  EXPECT_EQ(map_view.get_extent(), map.extent);
  EXPECT_EQ(map_view.get_next_layer_id(), id_cache.next_layer_id);
  EXPECT_EQ(map_view.get_next_object_id(), id_cache.next_object_id);
  EXPECT_EQ(map_view.layer_count(), count_layers(registry, map.root_layer));
  EXPECT_EQ(map_view.tileset_count(), map.attached_tilesets.size());
  // TODO component_count
}

}  // namespace tactile::test
