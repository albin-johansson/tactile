#include "core/algorithms/flood_fill.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

TEST(FloodFill, Flood)
{
  auto registry = Sys::MakeRegistry();

  const auto entity = Sys::AddTileLayer(registry);
  const auto& layer = registry.get<TileLayer>(entity);

  registry.ctx<ActiveLayer>().entity = entity;

  /* 0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0 */
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {i, j}));
    }
  }

  /* 0  0  0  0  0
     0  0  0  1  1
     0  0  0  1  0
     0  1  1  1  0
     1  0  0  0  0 */
  Sys::SetTileInLayer(registry, entity, {1, 3}, 1);
  Sys::SetTileInLayer(registry, entity, {1, 4}, 1);
  Sys::SetTileInLayer(registry, entity, {2, 3}, 1);
  Sys::SetTileInLayer(registry, entity, {3, 1}, 1);
  Sys::SetTileInLayer(registry, entity, {3, 2}, 1);
  Sys::SetTileInLayer(registry, entity, {3, 3}, 1);
  Sys::SetTileInLayer(registry, entity, {4, 0}, 1);

  {
    std::vector<MapPosition> affected;
    Flood(registry, entity, {4, 1}, 2, affected);
  }

  /* 0  0  0  0  0
     0  0  0  1  1
     0  0  0  1  2
     0  1  1  1  2
     1  2  2  2  2 */

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 0}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 1}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 2}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 3}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 4}));

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {1, 0}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {1, 1}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {1, 2}));
  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {1, 3}));
  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {1, 4}));

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {2, 0}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {2, 1}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {2, 2}));
  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {2, 3}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {2, 4}));

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {3, 0}));
  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {3, 1}));
  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {3, 2}));
  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {3, 3}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {3, 4}));

  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {4, 0}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 1}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 2}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 3}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 4}));

  {
    std::vector<MapPosition> affected;
    Flood(registry, entity, {3, 1}, 3, affected);
  }

  /* 0  0  0  0  0
     0  0  0  3  3
     0  0  0  3  2
     0  3  3  3  2
     1  2  2  2  2 */

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 0}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 1}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 2}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 3}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {0, 4}));

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {1, 0}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {1, 1}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {1, 2}));
  ASSERT_EQ(3, Sys::GetTileFromLayer(registry, entity, {1, 3}));
  ASSERT_EQ(3, Sys::GetTileFromLayer(registry, entity, {1, 4}));

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {2, 0}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {2, 1}));
  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {2, 2}));
  ASSERT_EQ(3, Sys::GetTileFromLayer(registry, entity, {2, 3}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {2, 4}));

  ASSERT_EQ(0, Sys::GetTileFromLayer(registry, entity, {3, 0}));
  ASSERT_EQ(3, Sys::GetTileFromLayer(registry, entity, {3, 1}));
  ASSERT_EQ(3, Sys::GetTileFromLayer(registry, entity, {3, 2}));
  ASSERT_EQ(3, Sys::GetTileFromLayer(registry, entity, {3, 3}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {3, 4}));

  ASSERT_EQ(1, Sys::GetTileFromLayer(registry, entity, {4, 0}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 1}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 2}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 3}));
  ASSERT_EQ(2, Sys::GetTileFromLayer(registry, entity, {4, 4}));
}