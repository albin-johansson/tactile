#include <gtest/gtest.h>

#include "io/parsing/map_parser.hpp"

using namespace Tactile;
using namespace Tactile::IO;

namespace {

void VerifyProperties(const MapData& data)
{
  ASSERT_EQ(7, data.properties.size());

  const auto& boolean = data.properties.at(0);
  ASSERT_EQ("boolean", boolean.name);
  ASSERT_EQ(PropertyType::Boolean, boolean.property.GetType());
  ASSERT_TRUE(boolean.property.AsBoolean());

  const auto& color = data.properties.at(1);
  ASSERT_EQ("cool color", color.name);
  ASSERT_EQ(PropertyType::Color, color.property.GetType());
  ASSERT_EQ(cen::color::from_argb("#FF55AAFF"), color.property.AsColor());

  const auto& floating = data.properties.at(2);
  ASSERT_EQ("floating", floating.name);
  ASSERT_EQ(PropertyType::Floating, floating.property.GetType());
  ASSERT_EQ(12.3f, floating.property.AsFloating());

  const auto& foo = data.properties.at(3);
  ASSERT_EQ("foo", foo.name);
  ASSERT_EQ(PropertyType::String, foo.property.GetType());
  ASSERT_EQ("bar", foo.property.AsString());

  const auto& integer = data.properties.at(4);
  ASSERT_EQ("integer", integer.name);
  ASSERT_EQ(PropertyType::Integer, integer.property.GetType());
  ASSERT_EQ(42, integer.property.AsInteger());

  const auto& object = data.properties.at(5);
  ASSERT_EQ("object ref", object.name);
  ASSERT_EQ(PropertyType::Object, object.property.GetType());
  ASSERT_EQ(1_obj, object.property.AsObject());

  const auto& path = data.properties.at(6);
  ASSERT_EQ("path", path.name);
  ASSERT_EQ(PropertyType::File, path.property.GetType());
  ASSERT_EQ(std::filesystem::path{"foo.bar"}, path.property.AsFile());
}

void VerifyLayers(const MapData& data)
{
  ASSERT_EQ(3, data.layers.size());

  {
    const auto& layer = data.layers.at(0);
    ASSERT_EQ("Tile Layer 1", layer.name);
    ASSERT_EQ(1_layer, layer.id);
    ASSERT_FLOAT_EQ(1.0f, layer.opacity);
    ASSERT_TRUE(layer.is_visible);
    ASSERT_TRUE(std::holds_alternative<TileLayerData>(layer.data));

    const auto& tileData = std::get<TileLayerData>(layer.data);
    ASSERT_EQ(5_row, tileData.row_count);
    ASSERT_EQ(7_col, tileData.col_count);

    ASSERT_EQ(5, tileData.tiles.size());
    ASSERT_EQ(7, tileData.tiles.at(0).size());
  }

  {
    const auto& layer = data.layers.at(1);
    ASSERT_EQ("Tile Layer 2", layer.name);
    ASSERT_EQ(2_layer, layer.id);
    ASSERT_FLOAT_EQ(0.8f, layer.opacity);
    ASSERT_TRUE(layer.is_visible);
    ASSERT_TRUE(std::holds_alternative<TileLayerData>(layer.data));

    const auto& tileData = std::get<TileLayerData>(layer.data);
    ASSERT_EQ(5_row, tileData.row_count);
    ASSERT_EQ(7_col, tileData.col_count);

    ASSERT_EQ(5, tileData.tiles.size());
    ASSERT_EQ(7, tileData.tiles.at(0).size());
  }

  {
    const auto& layer = data.layers.at(2);
    ASSERT_EQ("Object Layer 1", layer.name);
    ASSERT_EQ(3_layer, layer.id);
    ASSERT_FLOAT_EQ(1.0f, layer.opacity);
    ASSERT_TRUE(layer.is_visible);
    ASSERT_TRUE(std::holds_alternative<ObjectLayerData>(layer.data));

    const auto& objectData = std::get<ObjectLayerData>(layer.data);
    ASSERT_EQ(2, objectData.objects.size());

    const auto& rect = objectData.objects.at(0);
    ASSERT_EQ("Rect", rect.name);
    ASSERT_EQ(1_obj, rect.id);
    ASSERT_EQ(36, rect.x);
    ASSERT_EQ(40, rect.y);
    ASSERT_EQ(28, rect.width);
    ASSERT_EQ(30, rect.height);
    ASSERT_EQ("CustomType", rect.custom_type);
    ASSERT_TRUE(rect.visible);
    ASSERT_EQ(ObjectType::Rectangle, rect.type);
    ASSERT_TRUE(rect.properties.empty());

    const auto& point = objectData.objects.at(1);
    ASSERT_EQ("SomePoint", point.name);
    ASSERT_EQ(2_obj, point.id);
    ASSERT_EQ(143, point.x);
    ASSERT_EQ(47, point.y);
    ASSERT_EQ(0, point.width);
    ASSERT_EQ(0, point.height);
    ASSERT_EQ("CustomPoint", point.custom_type);
    ASSERT_TRUE(point.visible);
    ASSERT_EQ(ObjectType::Point, point.type);
    ASSERT_TRUE(point.properties.empty());
  }
}

void VerifyTilesets(const MapData& data, const std::filesystem::path& path)
{
  ASSERT_EQ(1, data.tilesets.size());

  const auto& tileset = data.tilesets.at(0);
  ASSERT_EQ("terrain", tileset.name);
  ASSERT_EQ(1_tile, tileset.first_id);
  ASSERT_EQ(32, tileset.tile_width);
  ASSERT_EQ(32, tileset.tile_height);
  ASSERT_TRUE(tileset.properties.empty());

  const auto directory = std::filesystem::absolute(path.parent_path());
  const auto expected =
      std::filesystem::weakly_canonical(directory / "../terrain.png");
  ASSERT_EQ(expected, tileset.absolute_image_path);

  ASSERT_EQ(1, tileset.tiles.size());

  const auto& tile = tileset.tiles.at(0);
  ASSERT_EQ(181_tile, tile.id);
  ASSERT_EQ(3, tile.animation.size());

  ASSERT_EQ(181_tile, tile.animation.at(0).tile);
  ASSERT_EQ(300, tile.animation.at(0).duration);

  ASSERT_EQ(182_tile, tile.animation.at(1).tile);
  ASSERT_EQ(300, tile.animation.at(1).duration);

  ASSERT_EQ(183_tile, tile.animation.at(2).tile);
  ASSERT_EQ(300, tile.animation.at(2).duration);
}

void VerifyMap(const std::filesystem::path& path)
{
  MapParser parser{path};
  ASSERT_EQ(ParseError::None, parser.GetError());

  const auto& data = parser.GetData();

  ASSERT_EQ(std::filesystem::absolute(path), data.absolute_path);
  ASSERT_EQ(4_layer, data.next_layer_id);
  ASSERT_EQ(3_obj, data.next_object_id);
  ASSERT_EQ(32, data.tile_width);
  ASSERT_EQ(32, data.tile_height);

  VerifyLayers(data);
  VerifyTilesets(data, path);
  VerifyProperties(data);
}

}  // namespace

TEST(ParseMapEmbeddedTilesets, JSON)
{
  VerifyMap("resources/json/embedded.json");
}

TEST(ParseMapEmbeddedTilesets, XML)
{
  VerifyMap("resources/xml/embedded.tmx");
}
