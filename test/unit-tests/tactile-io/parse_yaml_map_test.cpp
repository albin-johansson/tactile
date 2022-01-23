#include <filesystem>  // absolute

#include <tactile_def.hpp>
#include <tactile_io.hpp>

#include <gtest/gtest.h>

#include "io_test_helpers.hpp"

using namespace tactile;

namespace {

// clang-format off

constexpr auto raw_tileset_contents =
R"(version: 1
name: this-is-a-tileset
tile-count: 1000
tile-width: 32
tile-height: 36
column-count: 28
image-path: test-resources/terrain.png
image-width: 1024
image-height: 768
tiles:
  - id: 42
    animation:
      - tile: 42
        duration: 80
      - tile: 43
        duration: 100
      - tile: 44
        duration: 90
  - id: 123
    properties:
      - name: tile-float
        type: float
        value: 83.5
  - id: 87
    objects:
      - name: tile-object
        id: 829
        type: point
        x: 13
        y: 39
properties:
  - name: tileset-int
    type: int
    value: 101
)";

constexpr auto raw_map_contents =
R"(version: 1
row-count: 6
column-count: 8
tile-width: 32
tile-height: 48
next-layer-id: 7
next-object-id: 12
properties:
  - name: map-bool
    type: bool
    value: true
  - name: map-color
    type: color
    value: "#1A2B3C4D"
  - name: map-float
    type: float
    value: 12.3
  - name: map-string
    type: string
    value: bar
  - name: map-int
    type: int
    value: 123
  - name: map-object
    type: object
    value: 7
  - name: map-file
    type: file
    value: foo/bar.txt
layers:
  - name: Tile Layer 1
    id: 1
    type: tile-layer
    data: 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360 360
    properties:
      - name: layer-1-int
        type: int
        value: 42
  - name: Tile Layer 2
    id: 2
    opacity: 0.75
    type: tile-layer
    data: |
      123 123 123 123 123 123 123 123
      123 123 123 123 123 123 123 123
      123 123 123 123 123 123 123 123
      123 123 123 123 123 123 123 123
      123 123 123 123 123 123 123 123
      123 123 123 123 123 123 123 123
  - name: Object Layer 1
    id: 3
    type: object-layer
    objects:
      - id: 7
        type: rect
        name: Rectangle
        tag: Hitbox
        x: 36
        y: 40
        width: 28
        height: 30
        properties:
          - name: object-string
            type: string
            value: abc
  - name: Group Layer 1
    id: 4
    type: group-layer
    layers:
      - name: Tile Layer 3
        id: 5
        visible: false
        type: tile-layer
        data: 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101 101
tilesets:
  - first-global-id: 1
    path: generated_tileset.yaml
)";

// clang-format on

void ValidateTilesets(const IO::Map& map)
{
  ASSERT_EQ(1u, IO::GetTilesetCount(map));

  const auto& tileset = IO::GetTileset(map, 0u);
  ASSERT_STREQ("this-is-a-tileset", IO::GetName(tileset));

  ASSERT_EQ(1'000, IO::GetTileCount(tileset));
  ASSERT_EQ(32, IO::GetTileWidth(tileset));
  ASSERT_EQ(36, IO::GetTileHeight(tileset));
  ASSERT_EQ(28, IO::GetColumnCount(tileset));

  ASSERT_EQ(std::filesystem::absolute("test-resources/terrain.png"),
            IO::GetImagePath(tileset));
  ASSERT_EQ(1'024, IO::GetImageWidth(tileset));
  ASSERT_EQ(768, IO::GetImageHeight(tileset));

  ASSERT_EQ(3u, IO::GetTileInfoCount(tileset));

  {
    const auto& tile = IO::GetTileInfo(tileset, 0u);
    ASSERT_EQ(0u, IO::GetPropertyCount(tile));
    ASSERT_EQ(3u, IO::GetAnimationFrameCount(tile));
    ASSERT_EQ(0u, IO::GetObjectCount(tile));

    ASSERT_EQ(42, IO::GetTile(IO::GetAnimationFrame(tile, 0u)));
    ASSERT_EQ(80, IO::GetDuration(IO::GetAnimationFrame(tile, 0u)));

    ASSERT_EQ(43, IO::GetTile(IO::GetAnimationFrame(tile, 1u)));
    ASSERT_EQ(100, IO::GetDuration(IO::GetAnimationFrame(tile, 1u)));

    ASSERT_EQ(44, IO::GetTile(IO::GetAnimationFrame(tile, 2u)));
    ASSERT_EQ(90, IO::GetDuration(IO::GetAnimationFrame(tile, 2u)));
  }

  {
    const auto& tile = IO::GetTileInfo(tileset, 1u);
    ASSERT_EQ(1u, IO::GetPropertyCount(tile));
    ASSERT_EQ(0u, IO::GetAnimationFrameCount(tile));
    ASSERT_EQ(0u, IO::GetObjectCount(tile));

    ASSERT_TRUE(IO::HasPropertyWithName(tile, "tile-float"));
    ASSERT_FLOAT_EQ(83.5f, IO::GetFloat(tile, "tile-float"));
  }

  {
    const auto& tile = IO::GetTileInfo(tileset, 2u);
    ASSERT_EQ(0u, IO::GetPropertyCount(tile));
    ASSERT_EQ(0u, IO::GetAnimationFrameCount(tile));
    ASSERT_EQ(1u, IO::GetObjectCount(tile));

    const auto& object = IO::GetObject(tile, 0u);
    ASSERT_EQ(ObjectType::Point, IO::GetType(object));
    ASSERT_EQ(829, IO::GetId(object));
    ASSERT_STREQ("", IO::GetTag(object));
    ASSERT_FLOAT_EQ(13, IO::GetX(object));
    ASSERT_FLOAT_EQ(39, IO::GetY(object));
    ASSERT_FLOAT_EQ(0, IO::GetWidth(object));
    ASSERT_FLOAT_EQ(0, IO::GetHeight(object));
    ASSERT_TRUE(IO::IsVisible(object));
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

    ASSERT_EQ(1u, IO::GetPropertyCount(layer));
    ASSERT_TRUE(IO::HasPropertyWithName(layer, "layer-1-int"));
    ASSERT_EQ(42, IO::GetInt(layer, "layer-1-int"));

    ASSERT_TRUE(IO::IsTileLayer(layer));
    const auto& tileLayer = IO::GetTileLayer(layer);

    ASSERT_EQ(6u, IO::GetRowCount(tileLayer));
    ASSERT_EQ(8u, IO::GetColumnCount(tileLayer));

    ASSERT_EQ(360, IO::GetTile(tileLayer, 0u, 0u));
    ASSERT_EQ(360, IO::GetTile(tileLayer, 5u, 7u));
  }

  {
    const auto& layer = IO::GetLayer(map, 1u);
    ASSERT_STREQ("Tile Layer 2", IO::GetName(layer));

    ASSERT_EQ(LayerType::TileLayer, IO::GetType(layer));
    ASSERT_EQ(2, IO::GetId(layer));
    ASSERT_EQ(1u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(0.75f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));

    ASSERT_EQ(0u, IO::GetPropertyCount(layer));

    ASSERT_TRUE(IO::IsTileLayer(layer));
    const auto& tileLayer = IO::GetTileLayer(layer);

    ASSERT_EQ(6u, IO::GetRowCount(tileLayer));
    ASSERT_EQ(8u, IO::GetColumnCount(tileLayer));

    ASSERT_EQ(123, IO::GetTile(tileLayer, 0u, 0u));
    ASSERT_EQ(123, IO::GetTile(tileLayer, 5u, 7u));
  }

  {
    const auto& layer = IO::GetLayer(map, 2u);
    ASSERT_STREQ("Object Layer 1", IO::GetName(layer));

    ASSERT_EQ(LayerType::ObjectLayer, IO::GetType(layer));
    ASSERT_EQ(3, IO::GetId(layer));
    ASSERT_EQ(2u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));

    ASSERT_EQ(0u, IO::GetPropertyCount(layer));

    ASSERT_TRUE(IO::IsObjectLayer(layer));
    const auto& objectLayer = IO::GetObjectLayer(layer);

    ASSERT_EQ(1u, IO::GetObjectCount(objectLayer));

    const auto& object = IO::GetObject(objectLayer, 0u);
    ASSERT_EQ(ObjectType::Rectangle, IO::GetType(object));
    ASSERT_EQ(7, IO::GetId(object));
    ASSERT_STREQ("Rectangle", IO::GetName(object));
    ASSERT_STREQ("Hitbox", IO::GetTag(object));
    ASSERT_FLOAT_EQ(36, IO::GetX(object));
    ASSERT_FLOAT_EQ(40, IO::GetY(object));
    ASSERT_FLOAT_EQ(28, IO::GetWidth(object));
    ASSERT_FLOAT_EQ(30, IO::GetHeight(object));
    ASSERT_TRUE(IO::IsVisible(object));
  }

  {
    const auto& layer = IO::GetLayer(map, 3u);
    ASSERT_STREQ("Group Layer 1", IO::GetName(layer));

    ASSERT_EQ(LayerType::GroupLayer, IO::GetType(layer));
    ASSERT_EQ(4, IO::GetId(layer));
    ASSERT_EQ(3u, IO::GetIndex(layer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(layer));
    ASSERT_TRUE(IO::IsVisible(layer));

    ASSERT_EQ(0u, IO::GetPropertyCount(layer));

    ASSERT_TRUE(IO::IsGroupLayer(layer));
    const auto& groupLayer = IO::GetGroupLayer(layer);

    ASSERT_EQ(1u, IO::GetLayerCount(groupLayer));

    const auto& childLayer = IO::GetLayer(groupLayer, 0u);
    ASSERT_STREQ("Tile Layer 3", IO::GetName(childLayer));

    ASSERT_EQ(LayerType::TileLayer, IO::GetType(childLayer));
    ASSERT_EQ(5, IO::GetId(childLayer));
    ASSERT_EQ(0u, IO::GetIndex(childLayer));
    ASSERT_FLOAT_EQ(1.0f, IO::GetOpacity(childLayer));
    ASSERT_FALSE(IO::IsVisible(childLayer));

    ASSERT_EQ(0u, IO::GetPropertyCount(childLayer));

    ASSERT_TRUE(IO::IsTileLayer(childLayer));
    const auto& childTileLayer = IO::GetTileLayer(childLayer);

    ASSERT_EQ(6u, IO::GetRowCount(childTileLayer));
    ASSERT_EQ(8u, IO::GetColumnCount(childTileLayer));

    ASSERT_EQ(101, IO::GetTile(childTileLayer, 0u, 0u));
    ASSERT_EQ(101, IO::GetTile(childTileLayer, 5u, 7u));
  }
}

void ValidateMap(const IO::Map& map)
{
  ASSERT_EQ(6u, IO::GetRowCount(map));
  ASSERT_EQ(8u, IO::GetColumnCount(map));

  ASSERT_EQ(32, IO::GetTileWidth(map));
  ASSERT_EQ(48, IO::GetTileHeight(map));

  ASSERT_EQ(7, IO::GetNextLayerId(map));
  ASSERT_EQ(12, IO::GetNextObjectId(map));

  ASSERT_EQ(7u, IO::GetPropertyCount(map));

  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-string"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-int"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-float"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-bool"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-color"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-object"));
  ASSERT_TRUE(IO::HasPropertyWithName(map, "map-file"));

  ASSERT_TRUE(IO::GetBool(map, "map-bool"));
  ASSERT_EQ(123, IO::GetInt(map, "map-int"));
  ASSERT_EQ(7, IO::GetObject(map, "map-object"));
  ASSERT_FLOAT_EQ(12.3f, IO::GetFloat(map, "map-float"));
  ASSERT_STREQ("bar", IO::GetString(map, "map-string"));
  ASSERT_EQ(0x1A, IO::GetColor(map, "map-color").red);
  ASSERT_EQ(0x2B, IO::GetColor(map, "map-color").green);
  ASSERT_EQ(0x3C, IO::GetColor(map, "map-color").blue);
  ASSERT_EQ(0x4D, IO::GetColor(map, "map-color").alpha);

  ValidateLayers(map);
  ValidateTilesets(map);
}

}  // namespace

TEST(ParseYamlMap, Generated)
{
  WriteToFile("generated_tileset.yaml", raw_tileset_contents);
  WriteToFile("generated_map.yaml", raw_map_contents);

  IO::ParseError err{};
  const auto ptr = IO::ParseMap("generated_map.yaml", &err);
  ASSERT_EQ(IO::ParseError::None, err);
  ASSERT_TRUE(ptr);

  ValidateMap(*ptr);
}