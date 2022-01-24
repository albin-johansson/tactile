#include "core/algorithms/flood_fill.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "core/systems/registry_system.hpp"

using namespace tactile;

TEST(FloodFill, flood)
{
  auto registry = sys::MakeRegistry();

  const auto entity = sys::AddTileLayer(registry);

  registry.ctx<ActiveLayer>().entity = entity;

  /* 0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0
     0  0  0  0  0 */
  for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
      ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {i, j}));
    }
  }

  /* 0  0  0  0  0
     0  0  0  1  1
     0  0  0  1  0
     0  1  1  1  0
     1  0  0  0  0 */
  sys::SetTileInLayer(registry, entity, {1, 3}, 1);
  sys::SetTileInLayer(registry, entity, {1, 4}, 1);
  sys::SetTileInLayer(registry, entity, {2, 3}, 1);
  sys::SetTileInLayer(registry, entity, {3, 1}, 1);
  sys::SetTileInLayer(registry, entity, {3, 2}, 1);
  sys::SetTileInLayer(registry, entity, {3, 3}, 1);
  sys::SetTileInLayer(registry, entity, {4, 0}, 1);

  {
    std::vector<map_position> affected;
    flood(registry, entity, {4, 1}, 2, affected);
  }

  /* 0  0  0  0  0
     0  0  0  1  1
     0  0  0  1  2
     0  1  1  1  2
     1  2  2  2  2 */

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 0}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 1}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 2}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 3}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 4}));

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {1, 0}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {1, 1}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {1, 2}));
  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {1, 3}));
  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {1, 4}));

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {2, 0}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {2, 1}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {2, 2}));
  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {2, 3}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {2, 4}));

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {3, 0}));
  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {3, 1}));
  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {3, 2}));
  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {3, 3}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {3, 4}));

  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {4, 0}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 1}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 2}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 3}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 4}));

  {
    std::vector<map_position> affected;
    flood(registry, entity, {3, 1}, 3, affected);
  }

  /* 0  0  0  0  0
     0  0  0  3  3
     0  0  0  3  2
     0  3  3  3  2
     1  2  2  2  2 */

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 0}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 1}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 2}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 3}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {0, 4}));

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {1, 0}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {1, 1}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {1, 2}));
  ASSERT_EQ(3, sys::GetTileFromLayer(registry, entity, {1, 3}));
  ASSERT_EQ(3, sys::GetTileFromLayer(registry, entity, {1, 4}));

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {2, 0}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {2, 1}));
  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {2, 2}));
  ASSERT_EQ(3, sys::GetTileFromLayer(registry, entity, {2, 3}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {2, 4}));

  ASSERT_EQ(0, sys::GetTileFromLayer(registry, entity, {3, 0}));
  ASSERT_EQ(3, sys::GetTileFromLayer(registry, entity, {3, 1}));
  ASSERT_EQ(3, sys::GetTileFromLayer(registry, entity, {3, 2}));
  ASSERT_EQ(3, sys::GetTileFromLayer(registry, entity, {3, 3}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {3, 4}));

  ASSERT_EQ(1, sys::GetTileFromLayer(registry, entity, {4, 0}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 1}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 2}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 3}));
  ASSERT_EQ(2, sys::GetTileFromLayer(registry, entity, {4, 4}));
}