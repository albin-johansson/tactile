#include <tactile-base/tactile_std.hpp>

#include <gtest/gtest.h>

#include "io/map_parser.hpp"

using namespace Tactile;

TEST(TmxTileNodes, Parse)
{
  MapParser parser{"test-resources/xml/embedded_with_tile_nodes.tmx"};
  ASSERT_TRUE(parser);
  ASSERT_EQ(IO::ParseError::None, parser.GetError());

  const auto& data = parser.GetData();
  ASSERT_EQ(2, IO::GetLayerCount(data));

  {
    const auto& layer = IO::GetLayer(data, 0);
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));

    const auto& layerData = IO::GetTileLayer(layer);
    ASSERT_EQ(8, IO::GetRowCount(layerData));
    ASSERT_EQ(12, IO::GetColumnCount(layerData));

    ASSERT_EQ(360, IO::GetTile(layerData, 3, 8));
    ASSERT_EQ(empty_tile, IO::GetTile(layerData, 1, 9));
  }

  {
    const auto& layer = IO::GetLayer(data, 1);
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));

    const auto& layerData = IO::GetTileLayer(layer);
    ASSERT_EQ(8, IO::GetRowCount(layerData));
    ASSERT_EQ(12, IO::GetColumnCount(layerData));

    ASSERT_EQ(615, IO::GetTile(layerData, 4, 6));
    ASSERT_EQ(empty_tile, IO::GetTile(layerData, 3, 4));
  }
}
