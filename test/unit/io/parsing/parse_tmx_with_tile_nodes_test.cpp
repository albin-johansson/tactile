#include <gtest/gtest.h>

#include "io/map_parser.hpp"

using namespace Tactile;

TEST(TmxTileNodes, Parse)
{
  IO::MapParser parser{"resources/xml/embedded_with_tile_nodes.tmx"};
  ASSERT_TRUE(parser);
  ASSERT_EQ(IO::ParseError::None, parser.GetError());

  const auto& data = parser.GetData();
  ASSERT_EQ(2, data.layers.size());

  {
    const auto& layer = data.layers.at(0);
    ASSERT_EQ(LayerType::TileLayer, layer.type);

    const auto& layerData = std::get<IO::TileLayerData>(layer.data);
    ASSERT_EQ(8, layerData.row_count);
    ASSERT_EQ(12, layerData.col_count);

    ASSERT_EQ(360_tile, layerData.tiles.at(3).at(8));
    ASSERT_EQ(empty_tile, layerData.tiles.at(1).at(9));
  }

  {
    const auto& layer = data.layers.at(1);
    ASSERT_EQ(LayerType::TileLayer, layer.type);

    const auto& layerData = std::get<IO::TileLayerData>(layer.data);
    ASSERT_EQ(8, layerData.row_count);
    ASSERT_EQ(12, layerData.col_count);

    ASSERT_EQ(615_tile, layerData.tiles.at(4).at(6));
    ASSERT_EQ(empty_tile, layerData.tiles.at(3).at(4));
  }
}
