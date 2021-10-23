#include <gtest/gtest.h>

#include "io/map_parser.hpp"

using namespace Tactile;
using namespace Tactile::IO;

namespace {

void VerifyProperties(const Map& data)
{
  ASSERT_EQ(7, IO::GetPropertyCount(data));

  const auto& boolean = IO::GetProperty(data, 0);
  ASSERT_STREQ("boolean", IO::GetName(boolean));
  ASSERT_EQ(PropertyType::Boolean, IO::GetType(boolean));
  ASSERT_TRUE(IO::GetBool(boolean));

  const auto& color = IO::GetProperty(data, 1);
  ASSERT_STREQ("cool color", IO::GetName(color));
  ASSERT_EQ(PropertyType::Color, IO::GetType(color));
  ASSERT_EQ(0x55, IO::GetColor(color).red);
  ASSERT_EQ(0xAA, IO::GetColor(color).green);
  ASSERT_EQ(0xFF, IO::GetColor(color).blue);
  ASSERT_EQ(0xFF, IO::GetColor(color).alpha);

  const auto& floating = IO::GetProperty(data, 2);
  ASSERT_STREQ("floating", IO::GetName(floating));
  ASSERT_EQ(PropertyType::Floating, IO::GetType(floating));
  ASSERT_EQ(12.3f, IO::GetFloat(floating));

  const auto& foo = IO::GetProperty(data, 3);
  ASSERT_STREQ("foo", IO::GetName(foo));
  ASSERT_EQ(PropertyType::String, IO::GetType(foo));
  ASSERT_STREQ("bar", IO::GetString(foo));

  const auto& integer = IO::GetProperty(data, 4);
  ASSERT_STREQ("integer", IO::GetName(integer));
  ASSERT_EQ(PropertyType::Integer, IO::GetType(integer));
  ASSERT_EQ(42, IO::GetInt(integer));

  const auto& object = IO::GetProperty(data, 5);
  ASSERT_STREQ("object ref", IO::GetName(object));
  ASSERT_EQ(PropertyType::Object, IO::GetType(object));
  ASSERT_EQ(1, IO::GetObject(object));

  const auto& path = IO::GetProperty(data, 6);
  ASSERT_STREQ("path", IO::GetName(path));
  ASSERT_EQ(PropertyType::File, IO::GetType(path));
  ASSERT_EQ(std::filesystem::path{"foo.bar"}, IO::GetFile(path));
}

void VerifyLayers(const Map& data)
{
  ASSERT_EQ(3, IO::GetLayerCount(data));

  {
    const auto& layer = IO::GetLayer(data, 0);
    ASSERT_STREQ("Tile Layer 1", IO::GetName(layer));
    ASSERT_EQ(1, IO::GetId(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));
    ASSERT_TRUE(IO::IsTileLayer(layer));

    const auto& tileLayer = IO::GetTileLayer(layer);
    ASSERT_EQ(5, IO::GetRowCount(tileLayer));
    ASSERT_EQ(7, IO::GetColumnCount(tileLayer));
  }

  {
    const auto& layer = IO::GetLayer(data, 1);
    ASSERT_STREQ("Tile Layer 2", IO::GetName(layer));
    ASSERT_EQ(2, IO::GetId(layer));
    ASSERT_FLOAT_EQ(0.8f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));
    ASSERT_TRUE(IO::IsTileLayer(layer));

    const auto& tileLayer = IO::GetTileLayer(layer);
    ASSERT_EQ(5, IO::GetRowCount(tileLayer));
    ASSERT_EQ(7, IO::GetColumnCount(tileLayer));
  }

  {
    const auto& layer = IO::GetLayer(data, 2);
    ASSERT_STREQ("Object Layer 1", IO::GetName(layer));
    ASSERT_EQ(3, IO::GetId(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));
    ASSERT_TRUE(IO::IsObjectLayer(layer));

    const auto& objectLayer = IO::GetObjectLayer(layer);
    ASSERT_EQ(2, IO::GetObjectCount(objectLayer));

    const auto& rect = IO::GetObject(objectLayer, 0);
    ASSERT_STREQ("Rect", IO::GetName(rect));
    ASSERT_EQ(1, IO::GetId(rect));
    ASSERT_FLOAT_EQ(36, IO::GetX(rect));
    ASSERT_FLOAT_EQ(40, IO::GetY(rect));
    ASSERT_FLOAT_EQ(28, IO::GetWidth(rect));
    ASSERT_FLOAT_EQ(30, IO::GetHeight(rect));
    ASSERT_STREQ("CustomType", IO::GetTag(rect));
    ASSERT_TRUE(IO::IsVisible(rect));
    ASSERT_EQ(ObjectType::Rectangle, IO::GetType(rect));
    ASSERT_EQ(0, IO::GetPropertyCount(rect));

    const auto& point = IO::GetObject(objectLayer, 1);
    ASSERT_STREQ("SomePoint", IO::GetName(point));
    ASSERT_EQ(2, IO::GetId(point));
    ASSERT_FLOAT_EQ(143, IO::GetX(point));
    ASSERT_FLOAT_EQ(47, IO::GetY(point));
    ASSERT_FLOAT_EQ(0, IO::GetWidth(point));
    ASSERT_FLOAT_EQ(0, IO::GetHeight(point));
    ASSERT_STREQ("CustomPoint", IO::GetTag(point));
    ASSERT_TRUE(IO::IsVisible(point));
    ASSERT_EQ(ObjectType::Point, IO::GetType(point));
    ASSERT_EQ(0, IO::GetPropertyCount(point));
  }
}

void VerifyTilesets(const Map& data, const std::filesystem::path& path)
{
  ASSERT_EQ(1, IO::GetTilesetCount(data));

  const auto& tileset = IO::GetTileset(data, 0);
  ASSERT_STREQ("terrain", IO::GetName(tileset));
  ASSERT_EQ(1, IO::GetFirstGlobalId(tileset));
  ASSERT_EQ(32, IO::GetTileWidth(tileset));
  ASSERT_EQ(32, IO::GetTileHeight(tileset));
  ASSERT_EQ(0, IO::GetPropertyCount(tileset));

  const auto directory = std::filesystem::absolute(path.parent_path());
  const auto expected = std::filesystem::weakly_canonical(directory / "../terrain.png");
  ASSERT_EQ(expected, IO::GetImagePath(tileset));

  ASSERT_EQ(1024, IO::GetTileCount(tileset));
  ASSERT_EQ(1, IO::GetTileInfoCount(tileset));

  const auto& tile = IO::GetTileInfo(tileset, 0);
  ASSERT_EQ(181, IO::GetId(tile));
  ASSERT_EQ(3, IO::GetAnimationFrameCount(tile));

  ASSERT_EQ(181, IO::GetTile(IO::GetAnimationFrame(tile, 0)));
  ASSERT_EQ(300, IO::GetDuration(IO::GetAnimationFrame(tile, 0)));

  ASSERT_EQ(182, IO::GetTile(IO::GetAnimationFrame(tile, 1)));
  ASSERT_EQ(300, IO::GetDuration(IO::GetAnimationFrame(tile, 1)));

  ASSERT_EQ(183, IO::GetTile(IO::GetAnimationFrame(tile, 2)));
  ASSERT_EQ(300, IO::GetDuration(IO::GetAnimationFrame(tile, 2)));
}

void VerifyMap(const std::filesystem::path& path)
{
  MapParser parser{path};
  ASSERT_EQ(ParseError::None, parser.GetError());

  const auto& data = parser.GetData();

  ASSERT_EQ(std::filesystem::absolute(path), IO::GetAbsolutePath(data));
  ASSERT_EQ(4, IO::GetNextLayerId(data));
  ASSERT_EQ(3, IO::GetNextObjectId(data));
  ASSERT_EQ(32, IO::GetTileWidth(data));
  ASSERT_EQ(32, IO::GetTileHeight(data));

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
