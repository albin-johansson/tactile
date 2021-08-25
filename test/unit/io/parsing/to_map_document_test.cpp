#include "io/parsing/to_map_document.hpp"

#include <gtest/gtest.h>

#include "io/parsing/map_parser.hpp"

using namespace Tactile;
using namespace Tactile::IO;

TEST(ToMapDocument, Test)
{
  MapParser parser{"resources/json/embedded.json"};
  ASSERT_TRUE(parser);

  const auto document = ToMapDocument(parser.GetData());
  ASSERT_FALSE(document->CanUndo());
  ASSERT_FALSE(document->CanRedo());
  ASSERT_EQ("embedded.json", document->GetName());
  ASSERT_EQ(std::filesystem::absolute("resources/json/embedded.json"),
            document->GetAbsolutePath());

  const auto& map = document->GetMap();
  ASSERT_EQ(5_row, map.GetRowCount());
  ASSERT_EQ(7_col, map.GetColumnCount());
  ASSERT_EQ(4_layer, map.GetNextLayerId());
  ASSERT_EQ(3_obj, map.GetNextObjectId());
  ASSERT_EQ(1_layer, map.GetActiveLayerId());

  ASSERT_EQ(3, map.GetLayerCount());

  {
    const auto& layer = map.GetLayer(1_layer);
    ASSERT_EQ("Tile Layer 1", layer->GetName());
    ASSERT_EQ(LayerType::TileLayer, layer->GetType());
    ASSERT_EQ(1.0f, layer->GetOpacity());
    ASSERT_EQ(0, layer->GetPropertyCount());
    ASSERT_TRUE(layer->IsVisible());

    const auto* tileLayer = map.GetTileLayer(1_layer);
    ASSERT_TRUE(tileLayer);
    ASSERT_EQ(35, tileLayer->GetTileCount());
  }

  {
    const auto& layer = map.GetLayer(2_layer);
    ASSERT_EQ("Tile Layer 2", layer->GetName());
    ASSERT_EQ(LayerType::TileLayer, layer->GetType());
    ASSERT_EQ(0.8f, layer->GetOpacity());
    ASSERT_EQ(0, layer->GetPropertyCount());
    ASSERT_TRUE(layer->IsVisible());

    const auto* tileLayer = map.GetTileLayer(2_layer);
    ASSERT_TRUE(tileLayer);
    ASSERT_EQ(35, tileLayer->GetTileCount());
  }

  {
    const auto& layer = map.GetLayer(3_layer);
    ASSERT_EQ("Object Layer 1", layer->GetName());
    ASSERT_EQ(LayerType::ObjectLayer, layer->GetType());
    ASSERT_EQ(1.0f, layer->GetOpacity());
    ASSERT_EQ(0, layer->GetPropertyCount());
    ASSERT_TRUE(layer->IsVisible());

    const auto* objectLayer = map.GetObjectLayer(3_layer);
    ASSERT_TRUE(objectLayer);
    ASSERT_EQ(2, objectLayer->GetObjectCount());

    ASSERT_TRUE(map.HasObject(1_obj));
    ASSERT_TRUE(map.HasObject(2_obj));
    ASSERT_FALSE(map.HasObject(42_obj));

    const auto& rect = objectLayer->GetObject(1_obj);
    ASSERT_EQ("Rect", rect.GetName());
    ASSERT_EQ(36, rect.GetX());
    ASSERT_EQ(40, rect.GetY());
    ASSERT_EQ(28, rect.GetWidth());
    ASSERT_EQ(30, rect.GetHeight());
    ASSERT_EQ("CustomType", rect.GetCustomType());
    ASSERT_TRUE(rect.IsVisible());
    ASSERT_TRUE(rect.IsRectangle());
    ASSERT_FALSE(rect.IsPoint());

    const auto& point = objectLayer->GetObject(2_obj);
    ASSERT_EQ("SomePoint", point.GetName());
    ASSERT_EQ(143, point.GetX());
    ASSERT_EQ(47, point.GetY());
    ASSERT_EQ(0, point.GetWidth());
    ASSERT_EQ(0, point.GetHeight());
    ASSERT_EQ("CustomPoint", point.GetCustomType());
    ASSERT_TRUE(point.IsVisible());
    ASSERT_TRUE(point.IsPoint());
    ASSERT_FALSE(point.IsRectangle());
  }

  const auto& tilesets = document->GetTilesets();
  ASSERT_EQ(1, tilesets.GetSize());

  const auto* tileset = tilesets.GetActiveTileset();
  ASSERT_TRUE(tileset);
  ASSERT_EQ("terrain", tileset->GetName());
  ASSERT_EQ(1_tile, tileset->GetFirstId());
  ASSERT_EQ(32, tileset->GetRowCount());
  ASSERT_EQ(32, tileset->GetColumnCount());
  ASSERT_EQ(0, tileset->GetPropertyCount());
  ASSERT_EQ(1024, tileset->GetWidth());
  ASSERT_EQ(1024, tileset->GetHeight());
  ASSERT_EQ(1024, tileset->GetTileCount());

  ASSERT_EQ(7, document->GetPropertyCount());

  const auto& boolean = document->GetProperty("boolean");
  ASSERT_TRUE(boolean.IsBool());
  ASSERT_TRUE(boolean.AsBool());

  const auto& color = document->GetProperty("cool color");
  ASSERT_TRUE(color.IsColor());
  ASSERT_EQ(cen::color::from_argb("#FF55AAFF"), color.AsColor());

  const auto& floating = document->GetProperty("floating");
  ASSERT_TRUE(floating.IsFloat());
  ASSERT_FLOAT_EQ(12.3f, floating.AsFloat());

  const auto& integer = document->GetProperty("integer");
  ASSERT_TRUE(integer.IsInt());
  ASSERT_EQ(42, integer.AsInt());

  const auto& object = document->GetProperty("object ref");
  ASSERT_TRUE(object.IsObject());
  ASSERT_EQ(1_obj, object.AsObject());

  const auto& path = document->GetProperty("path");
  ASSERT_TRUE(path.IsFile());
  ASSERT_EQ(std::filesystem::path{"foo.bar"}, path.AsFile());
}
