#include "io/parsing/to_map_document.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/object_layer.hpp"
#include "core/components/property_context.hpp"
#include "core/components/texture.hpp"
#include "core/components/tile_layer.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "io/parsing/map_parser.hpp"

using namespace Tactile;

TEST(ToMapDocument, Test)
{
  IO::MapParser parser{"resources/json/embedded.json"};
  ASSERT_TRUE(parser);

  const auto document = IO::ToMapDocument(parser.GetData());
  ASSERT_FALSE(document.commands.CanUndo());
  ASSERT_FALSE(document.commands.CanRedo());

  const auto& registry = document.registry;
  const auto& map = registry.ctx<Map>();
  const auto& mapContext = registry.ctx<PropertyContext>();
  const auto& activeLayer = registry.ctx<ActiveLayer>();

  ASSERT_EQ("embedded.json", mapContext.name);

  ASSERT_EQ(std::filesystem::absolute("resources/json/embedded.json"),
            document.path);

  ASSERT_EQ(5_row, map.row_count);
  ASSERT_EQ(7_col, map.column_count);
  ASSERT_EQ(4_layer, map.next_layer_id);
  ASSERT_EQ(3_obj, map.next_object_id);
  ASSERT_EQ(1_layer, registry.get<Layer>(activeLayer.entity).id);

  ASSERT_EQ(3, registry.view<Layer>().size());

  {
    const auto entity = Sys::FindLayer(registry, 1_layer);
    const auto& layer = registry.get<Layer>(entity);
    const auto& context = registry.get<PropertyContext>(entity);

    ASSERT_EQ("Tile Layer 1", context.name);
    ASSERT_EQ(LayerType::TileLayer, layer.type);
    ASSERT_EQ(1.0f, layer.opacity);
    ASSERT_TRUE(context.properties.empty());
    ASSERT_TRUE(layer.visible);

    ASSERT_TRUE(registry.all_of<TileLayer>(entity));

    const auto& tileLayer = registry.get<TileLayer>(entity);
    ASSERT_EQ(35, tileLayer.matrix.size() * tileLayer.matrix.at(0).size());
  }

  {
    const auto entity = Sys::FindLayer(registry, 2_layer);
    const auto& layer = registry.get<Layer>(entity);
    const auto& context = registry.get<PropertyContext>(entity);

    ASSERT_EQ("Tile Layer 2", context.name);
    ASSERT_EQ(LayerType::TileLayer, layer.type);
    ASSERT_EQ(0.8f, layer.opacity);
    ASSERT_TRUE(context.properties.empty());
    ASSERT_TRUE(layer.visible);

    ASSERT_TRUE(registry.all_of<TileLayer>(entity));

    const auto& tileLayer = registry.get<TileLayer>(entity);
    ASSERT_EQ(35, tileLayer.matrix.size() * tileLayer.matrix.at(0).size());
  }

  {
    const auto entity = Sys::FindLayer(registry, 3_layer);
    const auto& layer = registry.get<Layer>(entity);
    const auto& layerContext = registry.get<PropertyContext>(entity);

    ASSERT_EQ("Object Layer 1", layerContext.name);
    ASSERT_EQ(LayerType::ObjectLayer, layer.type);
    ASSERT_EQ(1.0f, layer.opacity);
    ASSERT_TRUE(layerContext.properties.empty());
    ASSERT_TRUE(layer.visible);

    ASSERT_TRUE(registry.all_of<ObjectLayer>(entity));

    const auto& objectLayer = registry.get<ObjectLayer>(entity);
    ASSERT_EQ(2, objectLayer.objects.size());

    ASSERT_TRUE(Sys::HasObject(registry, entity, 1_obj));
    ASSERT_TRUE(Sys::HasObject(registry, entity, 2_obj));
    ASSERT_FALSE(Sys::HasObject(registry, entity, 42_obj));

    {
      const auto rectEntity = Sys::FindObject(registry, entity, 1_obj);
      const auto& rect = registry.get<Object>(rectEntity);
      const auto& rectContext = registry.get<PropertyContext>(rectEntity);

      ASSERT_EQ("Rect", rectContext.name);
      ASSERT_EQ(36, rect.x);
      ASSERT_EQ(40, rect.y);
      ASSERT_EQ(28, rect.width);
      ASSERT_EQ(30, rect.height);
      ASSERT_EQ("CustomType", rect.custom_type);
      ASSERT_TRUE(rect.visible);
      ASSERT_EQ(ObjectType::Rectangle, rect.type);
      ASSERT_NE(ObjectType::Point, rect.type);
    }

    {
      const auto pointEntity = Sys::FindObject(registry, entity, 2_obj);
      const auto& point = registry.get<Object>(pointEntity);
      const auto& pointContext = registry.get<PropertyContext>(pointEntity);

      ASSERT_EQ("SomePoint", pointContext.name);
      ASSERT_EQ(143, point.x);
      ASSERT_EQ(47, point.y);
      ASSERT_EQ(0, point.width);
      ASSERT_EQ(0, point.height);
      ASSERT_EQ("CustomPoint", point.custom_type);
      ASSERT_TRUE(point.visible);
      ASSERT_EQ(ObjectType::Point, point.type);
      ASSERT_NE(ObjectType::Rectangle, point.type);
    }
  }

  {
    ASSERT_EQ(1, registry.view<Tileset>().size());

    const auto& tilesetEntity = registry.ctx<ActiveTileset>().entity;
    const auto& tileset = registry.get<Tileset>(tilesetEntity);
    const auto& tilesetContext = registry.get<PropertyContext>(tilesetEntity);

    ASSERT_EQ("terrain", tilesetContext.name);
    ASSERT_EQ(1_tile, tileset.first_id);
    ASSERT_EQ(32, tileset.row_count);
    ASSERT_EQ(32, tileset.column_count);
    ASSERT_EQ(1024, tileset.tile_count);
    ASSERT_TRUE(tilesetContext.properties.empty());

    const auto& texture = registry.get<Texture>(tilesetEntity);
    ASSERT_EQ(1024, texture.width);
    ASSERT_EQ(1024, texture.height);
  }

  {
    ASSERT_EQ(7, mapContext.properties.size());

    {
      const auto propertyEntity = Sys::FindProperty(registry, "boolean");
      const auto& property = registry.get<Property>(propertyEntity);
      ASSERT_TRUE(property.value.IsBool());
      ASSERT_TRUE(property.value.AsBool());
    }

    {
      const auto propertyEntity = Sys::FindProperty(registry, "cool color");
      const auto& property = registry.get<Property>(propertyEntity);
      ASSERT_TRUE(property.value.IsColor());
      ASSERT_EQ(cen::color::from_argb("#FF55AAFF"), property.value.AsColor());
    }

    {
      const auto propertyEntity = Sys::FindProperty(registry, "floating");
      const auto& property = registry.get<Property>(propertyEntity);
      ASSERT_TRUE(property.value.IsFloat());
      ASSERT_FLOAT_EQ(12.3f, property.value.AsFloat());
    }

    {
      const auto propertyEntity = Sys::FindProperty(registry, "integer");
      const auto& property = registry.get<Property>(propertyEntity);
      ASSERT_TRUE(property.value.IsInt());
      ASSERT_EQ(42, property.value.AsInt());
    }

    {
      const auto propertyEntity = Sys::FindProperty(registry, "object ref");
      const auto& property = registry.get<Property>(propertyEntity);
      ASSERT_TRUE(property.value.IsObject());
      ASSERT_EQ(1_obj, property.value.AsObject());
    }

    {
      const auto propertyEntity = Sys::FindProperty(registry, "path");
      const auto& property = registry.get<Property>(propertyEntity);
      ASSERT_TRUE(property.value.IsFile());
      ASSERT_EQ(std::filesystem::path{"foo.bar"}, property.value.AsFile());
    }
  }
}
