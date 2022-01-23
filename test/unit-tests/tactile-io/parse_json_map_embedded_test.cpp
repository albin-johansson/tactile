#include <filesystem>  // absolute

#include <tactile_io.hpp>

#include <gtest/gtest.h>

#include "io_test_helpers.hpp"

using namespace tactile;

namespace {

// clang-format off

constexpr auto raw_map_data = R"({
  "type": "map",
  "tiledversion": "1.7.0",
  "version": "1.6",
  "compressionlevel": -1,
  "orientation": "orthogonal",
  "renderorder": "right-down",
  "infinite": false,
  "width": 5,
  "height": 4,
  "nextlayerid": 7,
  "nextobjectid": 8,
  "tilewidth": 48,
  "tileheight": 36,
  "tilesets": [
    {
      "type": "tileset",
      "firstgid": 24,
      "name": "woho",
      "tilewidth": 25,
      "tileheight": 16,
      "tilecount": 1024,
      "columns": 34,
      "margin": 0,
      "spacing": 0,
      "image": "test-resources/terrain.png",
      "imagewidth": 746,
      "imageheight": 827,
      "tiles": [
        {
          "id": 87,
          "animation": [
            {
              "tileid": 87,
              "duration": 56
            },
            {
              "tileid": 88,
              "duration": 128
            },
            {
              "tileid": 89,
              "duration": 74
            }
          ]
        },
        {
          "id": 924,
          "properties": [
            {
              "name": "tile-bool",
              "type": "bool",
              "value": true
            }
          ]
        }
      ],
      "properties": [
        {
          "name": "tileset-float",
          "type": "float",
          "value": 13.4
        },
        {
          "name": "tileset-int",
          "type": "int",
          "value": 1337
        }
      ]
    }
  ],
  "layers": [
    {
      "type": "tilelayer",
      "name": "Tile Layer 1",
      "id": 1,
      "opacity": 1.0,
      "visible": true,
      "x": 0,
      "y": 0,
      "width": 5,
      "height": 4,
      "data": [27, 27, 27, 27, 27,
               27, 27, 27, 27, 27,
               27, 27, 27, 27, 27,
               27, 27, 27, 27, 27]
    },
    {
      "type": "tilelayer",
      "name": "Tile Layer 2",
      "id": 2,
      "opacity": 0.6,
      "visible": true,
      "x": 0,
      "y": 0,
      "width": 5,
      "height": 4,
      "data": [154, 154, 154, 154, 154,
               154, 154, 154, 154, 154,
               154, 154, 154, 154, 154,
               154, 154, 154, 154, 154]
    },
    {
      "type": "objectgroup",
      "name": "Object Layer 1",
      "id": 3,
      "opacity": 0.8,
      "visible": false,
      "x": 0,
      "y": 0,
      "draworder": "topdown",
      "objects": [
        {
          "id": 6,
          "name": "Foo",
          "x": 293,
          "y": 196,
          "width": 64,
          "height": 198,
          "rotation": 0,
          "type": "tag",
          "visible": true
        },
        {
          "id": 7,
          "name": "Bar",
          "x": 574,
          "y": 341,
          "width": 0,
          "height": 0,
          "point": true,
          "rotation": 0,
          "type": "",
          "visible": true
        }
      ]
    },
    {
      "type": "group",
      "name": "Group Layer 1",
      "id": 4,
      "opacity": 1.0,
      "visible": true,
      "x": 0,
      "y": 0,
      "layers": [
        {
          "type": "tilelayer",
          "name": "Tile Layer 3",
          "id": 5,
          "opacity": 0.9,
          "visible": true,
          "x": 0,
          "y": 0,
          "width": 5,
          "height": 4,
          "data": [404, 404, 404, 404, 404,
                   404, 404, 404, 404, 404,
                   404, 404, 404, 404, 404,
                   404, 404, 404, 404, 404],
          "properties": [
            {
              "name": "layer-string",
              "type": "string",
              "value": "123456"
            }
          ]
        }
      ]
    }
  ],
  "properties": [
    {
      "name": "map-int",
      "type": "int",
      "value": 42
    },
    {
      "name": "map-float",
      "type": "float",
      "value": 72.4
    },
    {
      "name": "map-bool",
      "type": "bool",
      "value": false
    },
    {
      "name": "map-string",
      "type": "string",
      "value": "foobar"
    },
    {
      "name": "map-color",
      "type": "color",
      "value": "#7E32AB29"
    },
    {
      "name": "map-file",
      "type": "file",
      "value": "abc/def/ghi.txt"
    },
    {
      "name": "map-object",
      "type": "object",
      "value": 6
    }
  ]
})";

// clang-format on

void ValidateTilesets(const IO::Map& map)
{
  ASSERT_EQ(1u, IO::GetTilesetCount(map));
  const auto& tileset = IO::GetTileset(map, 0u);

  ASSERT_STREQ("woho", IO::GetName(tileset));
  ASSERT_EQ(24, IO::GetFirstGlobalId(tileset));

  ASSERT_EQ(25, IO::GetTileWidth(tileset));
  ASSERT_EQ(16, IO::GetTileHeight(tileset));
  ASSERT_EQ(1'024, IO::GetTileCount(tileset));

  ASSERT_EQ(746, IO::GetImageWidth(tileset));
  ASSERT_EQ(827, IO::GetImageHeight(tileset));
  ASSERT_EQ(std::filesystem::absolute("test-resources/terrain.png"),
            IO::GetImagePath(tileset));

  ASSERT_EQ(2u, IO::GetPropertyCount(tileset));

  ASSERT_TRUE(IO::HasPropertyWithName(tileset, "tileset-int"));
  ASSERT_TRUE(IO::HasPropertyWithName(tileset, "tileset-float"));

  ASSERT_EQ(1'337, IO::GetInt(tileset, "tileset-int"));
  ASSERT_FLOAT_EQ(13.4f, IO::GetFloat(tileset, "tileset-float"));

  ASSERT_EQ(2u, IO::GetTileInfoCount(tileset));

  {
    const auto& tile = IO::GetTileInfo(tileset, 0u);

    ASSERT_EQ(87, IO::GetId(tile));
    ASSERT_EQ(3u, IO::GetAnimationFrameCount(tile));
    ASSERT_EQ(0u, IO::GetPropertyCount(tile));

    ASSERT_EQ(87, IO::GetTile(IO::GetAnimationFrame(tile, 0u)));
    ASSERT_EQ(56, IO::GetDuration(IO::GetAnimationFrame(tile, 0u)));

    ASSERT_EQ(88, IO::GetTile(IO::GetAnimationFrame(tile, 1u)));
    ASSERT_EQ(128, IO::GetDuration(IO::GetAnimationFrame(tile, 1u)));

    ASSERT_EQ(89, IO::GetTile(IO::GetAnimationFrame(tile, 2u)));
    ASSERT_EQ(74, IO::GetDuration(IO::GetAnimationFrame(tile, 2u)));
  }

  {
    const auto& tile = IO::GetTileInfo(tileset, 1u);

    ASSERT_EQ(924, IO::GetId(tile));
    ASSERT_EQ(0u, IO::GetAnimationFrameCount(tile));
    ASSERT_EQ(1u, IO::GetPropertyCount(tile));

    ASSERT_TRUE(IO::HasPropertyWithName(tile, "tile-bool"));
    ASSERT_TRUE(IO::GetBool(tile, "tile-bool"));
  }
}

void ValidateLayers(const IO::Map& map)
{
  ASSERT_EQ(4u, IO::GetLayerCount(map));

  {
    const auto& layer = IO::GetLayer(map, 0u);
    ASSERT_STREQ("Tile Layer 1", IO::GetName(layer));
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));
    ASSERT_EQ(1, IO::GetId(layer));
    ASSERT_EQ(0u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));

    ASSERT_TRUE(IO::IsTileLayer(layer));
    const auto& tileLayer = IO::GetTileLayer(layer);

    ASSERT_EQ(4u, IO::GetRowCount(tileLayer));
    ASSERT_EQ(5u, IO::GetColumnCount(tileLayer));

    ASSERT_EQ(27, IO::GetTile(tileLayer, 0u, 0u));
    ASSERT_EQ(27, IO::GetTile(tileLayer, 3u, 4u));
  }

  {
    const auto& layer = IO::GetLayer(map, 1u);
    ASSERT_STREQ("Tile Layer 2", IO::GetName(layer));
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));
    ASSERT_EQ(2, IO::GetId(layer));
    ASSERT_EQ(1u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(0.6f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));

    ASSERT_TRUE(IO::IsTileLayer(layer));
    const auto& tileLayer = IO::GetTileLayer(layer);

    ASSERT_EQ(4u, IO::GetRowCount(tileLayer));
    ASSERT_EQ(5u, IO::GetColumnCount(tileLayer));

    ASSERT_EQ(154, IO::GetTile(tileLayer, 0u, 0u));
    ASSERT_EQ(154, IO::GetTile(tileLayer, 3u, 4u));
  }

  {
    const auto& layer = IO::GetLayer(map, 2u);
    ASSERT_STREQ("Object Layer 1", IO::GetName(layer));
    ASSERT_EQ(LayerType::ObjectLayer, IO::GetType(layer));
    ASSERT_EQ(3, IO::GetId(layer));
    ASSERT_EQ(2u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(0.8f, IO::GetOpacity(layer));
    ASSERT_FALSE(IO::IsVisible(layer));

    ASSERT_TRUE(IO::IsObjectLayer(layer));
    const auto& objectLayer = IO::GetObjectLayer(layer);

    ASSERT_EQ(2u, IO::GetObjectCount(objectLayer));

    {
      const auto& object = IO::GetObject(objectLayer, 0u);
      ASSERT_STREQ("Foo", IO::GetName(object));
      ASSERT_EQ(ObjectType::Rectangle, IO::GetType(object));
      ASSERT_EQ(6, IO::GetId(object));
      ASSERT_FLOAT_EQ(293, IO::GetX(object));
      ASSERT_FLOAT_EQ(196, IO::GetY(object));
      ASSERT_FLOAT_EQ(64, IO::GetWidth(object));
      ASSERT_FLOAT_EQ(198, IO::GetHeight(object));
      ASSERT_STREQ("tag", IO::GetTag(object));
      ASSERT_TRUE(IO::IsVisible(object));
    }

    {
      const auto& object = IO::GetObject(objectLayer, 1u);
      ASSERT_STREQ("Bar", IO::GetName(object));
      ASSERT_EQ(ObjectType::Point, IO::GetType(object));
      ASSERT_EQ(7, IO::GetId(object));
      ASSERT_FLOAT_EQ(574, IO::GetX(object));
      ASSERT_FLOAT_EQ(341, IO::GetY(object));
      ASSERT_FLOAT_EQ(0, IO::GetWidth(object));
      ASSERT_FLOAT_EQ(0, IO::GetHeight(object));
      ASSERT_STREQ("", IO::GetTag(object));
      ASSERT_TRUE(IO::IsVisible(object));
    }
  }

  {
    const auto& layer = IO::GetLayer(map, 3u);
    ASSERT_STREQ("Group Layer 1", IO::GetName(layer));
    ASSERT_EQ(LayerType::GroupLayer, IO::GetType(layer));
    ASSERT_EQ(4, IO::GetId(layer));
    ASSERT_EQ(3u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));

    ASSERT_TRUE(IO::IsGroupLayer(layer));
    const auto& groupLayer = IO::GetGroupLayer(layer);

    ASSERT_EQ(1u, IO::GetLayerCount(groupLayer));

    const auto& childLayer = IO::GetLayer(groupLayer, 0u);
    ASSERT_STREQ("Tile Layer 3", IO::GetName(childLayer));
    ASSERT_EQ(LayerType::TileLayer, IO::GetType(childLayer));
    ASSERT_EQ(5, IO::GetId(childLayer));
    ASSERT_EQ(0u, IO::GetIndex(childLayer));
    ASSERT_FLOAT_EQ(0.9f, IO::GetOpacity(childLayer));
    ASSERT_TRUE(IO::IsVisible(childLayer));

    ASSERT_EQ(1u, IO::GetPropertyCount(childLayer));
    ASSERT_TRUE(IO::HasPropertyWithName(childLayer, "layer-string"));
    ASSERT_STREQ("123456", IO::GetString(childLayer, "layer-string"));

    const auto& childTileLayer = IO::GetTileLayer(childLayer);
    ASSERT_EQ(4u, IO::GetRowCount(childTileLayer));
    ASSERT_EQ(5u, IO::GetColumnCount(childTileLayer));

    ASSERT_EQ(404, IO::GetTile(childTileLayer, 0u, 0u));
    ASSERT_EQ(404, IO::GetTile(childTileLayer, 3u, 4u));
  }
}

void ValidateMap(const IO::Map& map)
{
  ASSERT_EQ(4u, IO::GetRowCount(map));
  ASSERT_EQ(5u, IO::GetColumnCount(map));

  ASSERT_EQ(7, IO::GetNextLayerId(map));
  ASSERT_EQ(8, IO::GetNextObjectId(map));

  ASSERT_EQ(48, IO::GetTileWidth(map));
  ASSERT_EQ(36, IO::GetTileHeight(map));

  ASSERT_EQ(7u, IO::GetPropertyCount(map));

  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-int"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-float"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-bool"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-string"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-color"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-object"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-file"));

  ASSERT_EQ(42, IO::GetInt(map, "map-int"));
  ASSERT_FLOAT_EQ(72.4f, IO::GetFloat(map, "map-float"));
  ASSERT_FALSE(IO::GetBool(map, "map-bool"));
  ASSERT_STREQ("foobar", IO::GetString(map, "map-string"));
  ASSERT_EQ(0x32, IO::GetColor(map, "map-color").red);
  ASSERT_EQ(0xAB, IO::GetColor(map, "map-color").green);
  ASSERT_EQ(0x29, IO::GetColor(map, "map-color").blue);
  ASSERT_EQ(0x7E, IO::GetColor(map, "map-color").alpha);
  // TODO ASSERT_EQ(std::filesystem::absolute("abc/def/ghi.txt"),
  //            IO::GetFile(map, "map-file"));
  ASSERT_EQ(6, IO::GetObject(map, "map-object"));

  ValidateTilesets(map);
  ValidateLayers(map);
}

}  // namespace

TEST(ParseJsonMapEmbedded, Parse)
{
  WriteToFile("generated_map_embedded.json", raw_map_data);

  IO::ParseError err{};
  const auto ptr = IO::ParseMap("generated_map_embedded.json", &err);

  ASSERT_EQ(IO::ParseError::None, err);
  ASSERT_TRUE(ptr);

  ValidateMap(*ptr);
}