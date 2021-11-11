#include <gtest/gtest.h>

#include "io/map_parser.hpp"

using namespace Tactile;
using namespace Tactile::IO;

namespace {

void VerifyLayers(const Map& data)
{
  ASSERT_EQ(3, IO::GetLayerCount(data));

  {
    const auto& layer = IO::GetLayer(data, 0);
    ASSERT_STREQ("Tile Layer 1", IO::GetName(layer));
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));
    ASSERT_EQ(1, IO::GetId(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));
    ASSERT_TRUE(IO::IsTileLayer(layer));

    const auto& tileLayer = IO::GetTileLayer(layer);
    ASSERT_EQ(6, IO::GetRowCount(tileLayer));
    ASSERT_EQ(8, IO::GetColumnCount(tileLayer));
  }

  {
    const auto& layer = IO::GetLayer(data, 1);
    ASSERT_STREQ("Tile Layer 2", IO::GetName(layer));
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));
    ASSERT_EQ(2, IO::GetId(layer));
    ASSERT_FLOAT_EQ(0.83f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));
    ASSERT_TRUE(IO::IsTileLayer(layer));

    const auto& tileLayer = IO::GetTileLayer(layer);
    ASSERT_EQ(6, IO::GetRowCount(tileLayer));
    ASSERT_EQ(8, IO::GetColumnCount(tileLayer));

    ASSERT_EQ(2, IO::GetPropertyCount(layer));

    {
      const auto& property = IO::GetProperty(layer, 0);
      ASSERT_STREQ("a", IO::GetName(property));
      ASSERT_EQ(PropertyType::Integer, IO::GetType(property));
      ASSERT_EQ(42, IO::GetInt(property));
    }

    {
      const auto& property = IO::GetProperty(layer, 1);
      ASSERT_STREQ("b", IO::GetName(property));
      ASSERT_EQ(PropertyType::Floating, IO::GetType(property));
      ASSERT_FLOAT_EQ(2.5f, IO::GetFloat(property));
    }
  }

  {
    const auto& layer = IO::GetLayer(data, 2);
    ASSERT_STREQ("Object Layer 1", IO::GetName(layer));
    ASSERT_EQ(LayerType::ObjectLayer, IO::GetType(layer));
    ASSERT_EQ(3, IO::GetId(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));
    ASSERT_TRUE(IO::IsObjectLayer(layer));

    const auto& objectLayer = IO::GetObjectLayer(layer);
    ASSERT_EQ(2, IO::GetObjectCount(objectLayer));

    const auto& rect = IO::GetObject(objectLayer, 0);
    ASSERT_EQ(2, IO::GetId(rect));
    ASSERT_STREQ("Rect", IO::GetName(rect));
    ASSERT_STREQ("CustomType", IO::GetTag(rect));
    ASSERT_FLOAT_EQ(36, IO::GetX(rect));
    ASSERT_FLOAT_EQ(40, IO::GetY(rect));
    ASSERT_FLOAT_EQ(28, IO::GetWidth(rect));
    ASSERT_FLOAT_EQ(30, IO::GetHeight(rect));
    ASSERT_EQ(ObjectType::Rectangle, IO::GetType(rect));
    ASSERT_TRUE(IO::IsVisible(rect));
    ASSERT_EQ(1, IO::GetPropertyCount(rect));

    {
      const auto& property = IO::GetProperty(rect, 0);
      ASSERT_STREQ("foo", IO::GetName(property));
      ASSERT_EQ(PropertyType::String, IO::GetType(property));
      ASSERT_STREQ("bar", IO::GetString(property));
    }

    const auto& point = IO::GetObject(objectLayer, 1);
    ASSERT_EQ(3, IO::GetId(point));
    ASSERT_STREQ("SomePoint", IO::GetName(point));
    ASSERT_STREQ("CustomPoint", IO::GetTag(point));
    ASSERT_FLOAT_EQ(143, IO::GetX(point));
    ASSERT_FLOAT_EQ(47, IO::GetY(point));
    ASSERT_FLOAT_EQ(0, IO::GetWidth(point));
    ASSERT_FLOAT_EQ(0, IO::GetHeight(point));
    ASSERT_EQ(ObjectType::Point, IO::GetType(point));
    ASSERT_TRUE(IO::IsVisible(point));
    ASSERT_EQ(0, IO::GetPropertyCount(point));
  }
}

void VerifyTilesets(const Map& data)
{
  ASSERT_EQ(2, IO::GetTilesetCount(data));

  {
    const auto& tileset = IO::GetTileset(data, 0);
    ASSERT_STREQ("terrain", IO::GetName(tileset));
    ASSERT_EQ(1, IO::GetFirstGlobalId(tileset));
    ASSERT_EQ(1024, IO::GetTileCount(tileset));
    ASSERT_EQ(32, IO::GetTileWidth(tileset));
    ASSERT_EQ(32, IO::GetTileHeight(tileset));
    ASSERT_EQ(32, IO::GetColumnCount(tileset));
    ASSERT_EQ(1024, IO::GetImageWidth(tileset));
    ASSERT_EQ(1024, IO::GetImageHeight(tileset));
  }

  {
    const auto& tileset = IO::GetTileset(data, 1);
    ASSERT_STREQ("outside", IO::GetName(tileset));
    ASSERT_EQ(1025, IO::GetFirstGlobalId(tileset));
    ASSERT_EQ(1024, IO::GetTileCount(tileset));
    ASSERT_EQ(32, IO::GetTileWidth(tileset));
    ASSERT_EQ(32, IO::GetTileHeight(tileset));
    ASSERT_EQ(32, IO::GetColumnCount(tileset));
    ASSERT_EQ(1024, IO::GetImageWidth(tileset));
    ASSERT_EQ(1024, IO::GetImageHeight(tileset));
  }
}

void VerifyProperties(const Map& data)
{
  ASSERT_EQ(7, IO::GetPropertyCount(data));

  {
    const auto& property = IO::GetProperty(data, 0);
    ASSERT_STREQ("boolean", IO::GetName(property));
    ASSERT_EQ(PropertyType::Boolean, IO::GetType(property));
    ASSERT_TRUE(IO::GetBool(property));
  }

  {
    const auto& property = IO::GetProperty(data, 1);
    ASSERT_STREQ("cool color", IO::GetName(property));
    ASSERT_EQ(PropertyType::Color, IO::GetType(property));
    ASSERT_EQ(0x1A, IO::GetColor(property).red);
    ASSERT_EQ(0x2B, IO::GetColor(property).green);
    ASSERT_EQ(0x3C, IO::GetColor(property).blue);
    ASSERT_EQ(0x4D, IO::GetColor(property).alpha);
  }

  {
    const auto& property = IO::GetProperty(data, 2);
    ASSERT_STREQ("floating", IO::GetName(property));
    ASSERT_EQ(PropertyType::Floating, IO::GetType(property));
    ASSERT_FLOAT_EQ(12.3f, IO::GetFloat(property));
  }

  {
    const auto& property = IO::GetProperty(data, 3);
    ASSERT_STREQ("foo", IO::GetName(property));
    ASSERT_EQ(PropertyType::String, IO::GetType(property));
    ASSERT_STREQ("bar", IO::GetString(property));
  }

  {
    const auto& property = IO::GetProperty(data, 4);
    ASSERT_STREQ("integer", IO::GetName(property));
    ASSERT_EQ(PropertyType::Integer, IO::GetType(property));
    ASSERT_EQ(42, IO::GetInt(property));
  }

  {
    const auto& property = IO::GetProperty(data, 5);
    ASSERT_STREQ("object ref", IO::GetName(property));
    ASSERT_EQ(PropertyType::Object, IO::GetType(property));
    ASSERT_EQ(1, IO::GetObject(property));
  }

  {
    const auto& property = IO::GetProperty(data, 6);
    ASSERT_STREQ("path", IO::GetName(property));
    ASSERT_EQ(PropertyType::File, IO::GetType(property));
    ASSERT_EQ(std::filesystem::path{"foo.bar"}, IO::GetFile(property));
  }
}

void VerifyMap(const std::filesystem::path& path)
{
  MapParser parser{path};
  ASSERT_EQ(ParseError::None, parser.GetError());

  const auto& data = parser.GetData();
  ASSERT_EQ(6, IO::GetRowCount(data));
  ASSERT_EQ(8, IO::GetColumnCount(data));
  ASSERT_EQ(4, IO::GetNextLayerId(data));
  ASSERT_EQ(4, IO::GetNextObjectId(data));
  ASSERT_EQ(32, IO::GetTileWidth(data));
  ASSERT_EQ(32, IO::GetTileHeight(data));
  ASSERT_EQ(std::filesystem::absolute(path), IO::GetPath(data));

  VerifyTilesets(data);
  VerifyLayers(data);
  VerifyProperties(data);
}

}  // namespace

TEST(ParseMapExternalTilesets, JSON)
{
  VerifyMap("test-resources/json/external.json");
}

TEST(ParseMapExternalTilesets, XML)
{
  VerifyMap("test-resources/xml/external.tmx");
}

TEST(ParseMapExternalTilesets, YAML)
{
  VerifyMap("test-resources/yaml/map.yaml");
}
