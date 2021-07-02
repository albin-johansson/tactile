#include "core/map/map.hpp"

#include <gtest/gtest.h>

#include "core/algorithms/invoke_n.hpp"
#include "core/tactile_error.hpp"

using namespace Tactile;

TEST(Map, Defaults)
{
  const Map map;
  ASSERT_EQ(0, map.GetLayerCount());
  ASSERT_FALSE(map.GetActiveLayerId());

  ASSERT_EQ(1_obj, map.GetNextObjectId());
  ASSERT_EQ(1_layer, map.GetNextLayerId());
}

TEST(Map, DimensionConstructor)
{
  // Invalid dimensions
  ASSERT_THROW(Map(0_row, 1_col), TactileError);
  ASSERT_THROW(Map(1_row, 0_col), TactileError);
  ASSERT_THROW(Map(0_row, 0_col), TactileError);
  ASSERT_THROW(Map(-1_row, -1_col), TactileError);

  // Valid dimensions
  ASSERT_NO_THROW(Map(1_row, 1_col));

  const auto rows = 7_row;
  const auto cols = 5_col;

  const Map map{rows, cols};
  ASSERT_EQ(rows, map.GetRowCount());
  ASSERT_EQ(cols, map.GetColumnCount());

  ASSERT_EQ(1, map.GetLayerCount());
  ASSERT_EQ(2_layer, map.GetNextLayerId());
  ASSERT_EQ(1_layer, map.GetNextObjectId());

  const auto& layer = map.GetLayer(1_layer);
  ASSERT_EQ(LayerType::TileLayer, layer->GetType());
}

TEST(Map, EachLayer)
{
  Map map;

  map.AddTileLayer();
  map.AddTileLayer();
  map.AddTileLayer();

  int count{};
  map.EachLayer([&](const layer_id id, const SharedLayer& layer) { ++count; });

  ASSERT_EQ(3, count);
}

TEST(Map, RemoveOccurrences)
{
  Map map{4_row, 4_col};

  const auto id = map.GetNextLayerId();
  auto layer = map.MakeTileLayer();

  std::vector<MapPosition> positions;
  layer->Flood({}, 1_tile, positions);

  map.AddLayer(id, layer);
  map.SelectLayer(id);
  map.RemoveOccurrences(1_tile);

  layer->Each([](const tile_id tile) { ASSERT_EQ(empty_tile, tile); });
}

TEST(Map, AddTileLayer)
{
  Map map{5_row, 5_col};
  ASSERT_EQ(1, map.GetLayerCount());

  const auto id = map.AddTileLayer();
  ASSERT_NE(id, map.GetActiveLayerId());  // New layer shouldn't be active

  ASSERT_TRUE(map.GetTileLayer(id));
  ASSERT_FALSE(map.GetObjectLayer(id));

  ASSERT_EQ(2, map.GetLayerCount());
  ASSERT_EQ(LayerType::TileLayer, map.GetLayer(id)->GetType());
}

TEST(Map, AddObjectLayer)
{
  Map map{5_row, 5_col};
  ASSERT_EQ(1, map.GetLayerCount());

  const auto id = map.AddObjectLayer();
  ASSERT_NE(id, map.GetActiveLayerId());  // New layer shouldn't be active

  ASSERT_TRUE(map.GetObjectLayer(id));
  ASSERT_FALSE(map.GetTileLayer(id));

  ASSERT_EQ(2, map.GetLayerCount());
  ASSERT_EQ(LayerType::ObjectLayer, map.GetLayer(id)->GetType());
}

TEST(Map, DuplicateLayer)
{
  Map map{4_row, 8_col};

  const auto& [id, layer] = map.DuplicateLayer(1_layer);
  ASSERT_TRUE(map.HasLayer(1_layer));
  ASSERT_TRUE(map.HasLayer(2_layer));
  ASSERT_EQ(2, map.GetLayerCount());

  ASSERT_EQ(2_layer, id);
  ASSERT_EQ(1.0f, layer->GetOpacity());
  ASSERT_TRUE(layer->IsVisible());
  ASSERT_EQ(LayerType::TileLayer, layer->GetType());
}

TEST(Map, RemoveLayer)
{
  Map map;

  const auto a = map.AddTileLayer();
  const auto b = map.AddTileLayer();
  ASSERT_EQ(2, map.GetLayerCount());

  ASSERT_NO_THROW(map.RemoveLayer(b));
  ASSERT_FALSE(map.HasLayer(b));
  ASSERT_TRUE(map.HasLayer(a));
  ASSERT_EQ(1, map.GetLayerCount());

  map.SelectLayer(a);
  map.RemoveLayer(a);
  ASSERT_EQ(0, map.GetLayerCount());
  ASSERT_FALSE(map.HasLayer(a));
  ASSERT_FALSE(map.HasLayer(b));
  ASSERT_FALSE(map.GetActiveLayerId());
}

TEST(Map, TakeLayer)
{
  Map map;

  const auto id = map.AddObjectLayer();
  ASSERT_TRUE(map.HasLayer(id));

  map.SelectLayer(id);
  ASSERT_EQ(id, map.GetActiveLayerId());

  auto layer = map.TakeLayer(id);
  ASSERT_FALSE(map.HasLayer(id));
  ASSERT_FALSE(map.GetActiveLayerId());
  ASSERT_EQ(LayerType::ObjectLayer, layer->GetType());
}

TEST(Map, SelectLayer)
{
  Map map;
  ASSERT_FALSE(map.GetActiveLayerId());

  const auto a = map.AddTileLayer();
  const auto b = map.AddObjectLayer();

  ASSERT_FALSE(map.GetActiveLayerId());

  map.SelectLayer(a);
  ASSERT_EQ(a, map.GetActiveLayerId());

  map.SelectLayer(b);
  ASSERT_EQ(b, map.GetActiveLayerId());

  ASSERT_NO_THROW(map.SelectLayer(b));
  ASSERT_EQ(b, map.GetActiveLayerId());

  map.RemoveLayer(b);
  ASSERT_FALSE(map.GetActiveLayerId());

  map.SelectLayer(a);
  ASSERT_EQ(a, map.GetActiveLayerId());
}

TEST(Map, AddRow)
{
  Map map{5_row, 5_col};
  ASSERT_EQ(5_row, map.GetRowCount());
  ASSERT_EQ(5_col, map.GetColumnCount());

  map.AddRow(empty_tile);
  ASSERT_EQ(6_row, map.GetRowCount());
  ASSERT_EQ(5_col, map.GetColumnCount());
}

TEST(Map, AddColumn)
{
  Map map{3_row, 6_col};
  ASSERT_EQ(3_row, map.GetRowCount());
  ASSERT_EQ(6_col, map.GetColumnCount());

  map.AddColumn(empty_tile);
  ASSERT_EQ(3_row, map.GetRowCount());
  ASSERT_EQ(7_col, map.GetColumnCount());
}

TEST(Map, RemoveRow)
{
  Map map{4_row, 10_col};
  ASSERT_EQ(4_row, map.GetRowCount());

  map.RemoveRow();
  ASSERT_EQ(3_row, map.GetRowCount());

  InvokeN(20, [&] { map.RemoveRow(); });
  ASSERT_EQ(1_row, map.GetRowCount());
}

TEST(Map, RemoveColumn)
{
  Map map{7_row, 9_col};

  map.RemoveColumn();
  ASSERT_EQ(8_col, map.GetColumnCount());

  InvokeN(20, [&] { map.RemoveColumn(); });
  ASSERT_EQ(1_col, map.GetColumnCount());
}

TEST(Map, SetNextLayerId)
{
  Map map;
  ASSERT_EQ(1_layer, map.GetNextLayerId());

  map.SetNextLayerId(7_layer);
  ASSERT_EQ(7_layer, map.GetNextLayerId());

  map.SetNextLayerId(4_layer);
  ASSERT_EQ(4_layer, map.GetNextLayerId());
}

TEST(Map, SetNextObjectId)
{
  Map map;
  ASSERT_EQ(1_obj, map.GetNextObjectId());

  map.SetNextObjectId(42_obj);
  ASSERT_EQ(42_obj, map.GetNextObjectId());

  map.SetNextObjectId(6_obj);
  ASSERT_EQ(6_obj, map.GetNextObjectId());
}

TEST(Map, SetRowCount)
{
  Map map{4_row, 4_col};

  map.SetRowCount(12_row);
  ASSERT_EQ(12_row, map.GetRowCount());
  ASSERT_EQ(4_col, map.GetColumnCount());

  map.SetRowCount(0_row);
  ASSERT_EQ(1_row, map.GetRowCount());
  ASSERT_EQ(4_col, map.GetColumnCount());

  map.SetRowCount(-1_row);
  ASSERT_EQ(1_row, map.GetRowCount());
  ASSERT_EQ(4_col, map.GetColumnCount());
}

TEST(Map, SetColumnCount)
{
  Map map{4_row, 4_col};

  map.SetColumnCount(8_col);
  ASSERT_EQ(4_row, map.GetRowCount());
  ASSERT_EQ(8_col, map.GetColumnCount());

  map.SetColumnCount(0_col);
  ASSERT_EQ(4_row, map.GetRowCount());
  ASSERT_EQ(1_col, map.GetColumnCount());

  map.SetColumnCount(-1_col);
  ASSERT_EQ(4_row, map.GetRowCount());
  ASSERT_EQ(1_col, map.GetColumnCount());
}

TEST(Map, SetVisible)
{
  Map map;

  const auto a = map.AddTileLayer();
  const auto b = map.AddObjectLayer();

  ASSERT_TRUE(map.IsVisible(a));
  ASSERT_TRUE(map.IsVisible(b));

  map.SetVisible(a, false);
  ASSERT_FALSE(map.IsVisible(a));
  ASSERT_TRUE(map.IsVisible(b));

  map.SetVisible(a, true);
  ASSERT_TRUE(map.IsVisible(a));
  ASSERT_TRUE(map.IsVisible(b));
}

TEST(Map, SetOpacity)
{
  Map map;

  const auto a = map.AddTileLayer();
  const auto b = map.AddObjectLayer();

  ASSERT_EQ(1.0f, map.GetOpacity(a));
  ASSERT_EQ(1.0f, map.GetOpacity(b));

  map.SetOpacity(b, 0.2f);
  ASSERT_EQ(1.0f, map.GetOpacity(a));
  ASSERT_EQ(0.2f, map.GetOpacity(b));
}

TEST(Map, SetName)
{
  Map map;

  const auto a = map.AddTileLayer();
  const auto b = map.AddTileLayer();

  map.SetName(a, "foo");
  map.SetName(b, "bar");

  ASSERT_EQ("foo", map.GetName(a));
  ASSERT_EQ("bar", map.GetName(b));
}

TEST(Map, MoveLayerBack)
{
  Map map;

  const auto a = map.AddTileLayer();
  const auto b = map.AddTileLayer();
  const auto c = map.AddTileLayer();

  ASSERT_EQ(0, map.IndexOf(a));
  ASSERT_EQ(1, map.IndexOf(b));
  ASSERT_EQ(2, map.IndexOf(c));

  // Moving the first layer back should have no effect
  map.MoveLayerDown(a);
  ASSERT_EQ(0, map.IndexOf(a));
  ASSERT_EQ(1, map.IndexOf(b));
  ASSERT_EQ(2, map.IndexOf(c));

  // [ a, b, c ] -> [ b, a, c ]
  map.MoveLayerDown(b);
  ASSERT_EQ(0, map.IndexOf(b));
  ASSERT_EQ(1, map.IndexOf(a));
  ASSERT_EQ(2, map.IndexOf(c));

  // [ b, a, c ] -> [ b, c, a ]
  map.MoveLayerDown(c);
  ASSERT_EQ(0, map.IndexOf(b));
  ASSERT_EQ(1, map.IndexOf(c));
  ASSERT_EQ(2, map.IndexOf(a));
}

TEST(Map, MoveLayerForward)
{
  Map map;

  const auto a = map.AddTileLayer();
  const auto b = map.AddTileLayer();
  const auto c = map.AddTileLayer();

  ASSERT_EQ(0, map.IndexOf(a));
  ASSERT_EQ(1, map.IndexOf(b));
  ASSERT_EQ(2, map.IndexOf(c));

  // Moving the last layer forward should have no effect
  map.MoveLayerUp(c);
  ASSERT_EQ(0, map.IndexOf(a));
  ASSERT_EQ(1, map.IndexOf(b));
  ASSERT_EQ(2, map.IndexOf(c));

  // [ a, b, c ] -> [ a, c, b ]
  map.MoveLayerUp(b);
  ASSERT_EQ(0, map.IndexOf(a));
  ASSERT_EQ(1, map.IndexOf(c));
  ASSERT_EQ(2, map.IndexOf(b));

  // [ a, c, b ] -> [ c, a, b ]
  map.MoveLayerUp(a);
  ASSERT_EQ(0, map.IndexOf(c));
  ASSERT_EQ(1, map.IndexOf(a));
  ASSERT_EQ(2, map.IndexOf(b));
}

TEST(Map, MakeTileLayer)
{
  Map map;
  ASSERT_EQ(0, map.GetLayerCount());
  ASSERT_EQ(1_layer, map.GetNextLayerId());

  auto layer = map.MakeTileLayer();
  ASSERT_EQ(LayerType::TileLayer, layer->GetType());
  ASSERT_EQ(0, map.GetLayerCount());
  ASSERT_EQ(2_layer, map.GetNextLayerId());
  ASSERT_FALSE(map.HasLayer(1_layer));
}

TEST(Map, MakeObjectLayer)
{
  Map map;
  ASSERT_EQ(0, map.GetLayerCount());
  ASSERT_EQ(1_layer, map.GetNextLayerId());

  auto layer = map.MakeObjectLayer();
  ASSERT_EQ(LayerType::ObjectLayer, layer->GetType());
  ASSERT_EQ(0, map.GetLayerCount());
  ASSERT_EQ(2_layer, map.GetNextLayerId());
  ASSERT_FALSE(map.HasLayer(1_layer));
}

TEST(Map, InBounds)
{
  const Map map{4_row, 5_col};

  ASSERT_FALSE(map.InBounds({4_row, 0_col}));
  ASSERT_FALSE(map.InBounds({0_row, 5_col}));
  ASSERT_FALSE(map.InBounds({4_row, 5_col}));

  ASSERT_TRUE(map.InBounds({2_row, 2_col}));
  ASSERT_TRUE(map.InBounds({0_row, 0_col}));
  ASSERT_TRUE(map.InBounds({0_row, 4_col}));
  ASSERT_TRUE(map.InBounds({3_row, 0_col}));
  ASSERT_TRUE(map.InBounds({3_row, 4_col}));
}
