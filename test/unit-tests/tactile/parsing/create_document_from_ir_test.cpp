#include "io/create_document_from_ir.hpp"

#include <gtest/gtest.h>

#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/components/property_context.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/map.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/object_layer_system.hpp"
#include "core/systems/property_system.hpp"
#include "core/utils/texture_manager.hpp"
#include "io/map_parser.hpp"

using namespace tactile;

namespace {

inline constexpr entt::entity gNullEntity = entt::null;

void CheckMapAttributes(const entt::registry& registry)
{
  const auto& map = registry.ctx<Map>();
  ASSERT_EQ(32, map.tile_width);
  ASSERT_EQ(32, map.tile_height);
  ASSERT_EQ(6, map.row_count);
  ASSERT_EQ(8, map.column_count);
  ASSERT_EQ(4, map.next_layer_id);
  ASSERT_EQ(4, map.next_object_id);

  const auto& context = registry.ctx<PropertyContext>();
  ASSERT_EQ("map.yaml", context.name);
  ASSERT_EQ(7, context.properties.size());
  ASSERT_EQ(2, context.components.size());

  {
    const auto propertyEntity = sys::FindProperty(registry, context, "boolean");
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_TRUE(property.value.is_bool());
    ASSERT_TRUE(property.value.as_bool());
  }

  {
    const auto propertyEntity = sys::FindProperty(registry, context, "cool color");
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_TRUE(property.value.is_color());
    ASSERT_EQ(cen::color::from_rgba("#1A2B3C4D"), property.value.as_color());
  }

  {
    const auto propertyEntity = sys::FindProperty(registry, context, "floating");
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_TRUE(property.value.is_float());
    ASSERT_FLOAT_EQ(12.3f, property.value.as_float());
  }

  {
    const auto propertyEntity = sys::FindProperty(registry, context, "integer");
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_TRUE(property.value.is_int());
    ASSERT_EQ(42, property.value.as_int());
  }

  {
    const auto propertyEntity = sys::FindProperty(registry, context, "object ref");
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_TRUE(property.value.is_object());
    ASSERT_EQ(1, property.value.as_object());
  }

  {
    const auto propertyEntity = sys::FindProperty(registry, context, "path");
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_TRUE(property.value.is_file());
    ASSERT_EQ(std::filesystem::path{"foo.bar"}, property.value.as_file());
  }
}

void CheckTileLayer1(const entt::registry& registry)
{
  const auto entity = sys::FindLayer(registry, 1);
  ASSERT_NE(entity, gNullEntity);

  const auto& context = registry.get<PropertyContext>(entity);
  ASSERT_EQ("Tile Layer 1", context.name);
  ASSERT_TRUE(context.properties.empty());
  ASSERT_TRUE(context.components.empty());

  const auto& layer = registry.get<Layer>(entity);
  ASSERT_EQ(LayerType::TileLayer, layer.type);
  ASSERT_EQ(1.0f, layer.opacity);
  ASSERT_TRUE(layer.visible);

  ASSERT_TRUE(registry.all_of<TileLayer>(entity));
  const auto& tileLayer = registry.get<TileLayer>(entity);

  ASSERT_EQ(48, tileLayer.matrix.size() * tileLayer.matrix.at(0).size());
}

void CheckTileLayer2(const entt::registry& registry)
{
  const auto entity = sys::FindLayer(registry, 2);
  ASSERT_NE(entity, gNullEntity);

  const auto& context = registry.get<PropertyContext>(entity);
  ASSERT_EQ("Tile Layer 2", context.name);
  ASSERT_EQ(2, context.properties.size());
  ASSERT_TRUE(context.components.empty());

  const auto& layer = registry.get<Layer>(entity);
  ASSERT_EQ(LayerType::TileLayer, layer.type);
  ASSERT_FLOAT_EQ(0.83f, layer.opacity);
  ASSERT_TRUE(layer.visible);

  {
    ASSERT_TRUE(registry.all_of<TileLayer>(entity));

    const auto& tileLayer = registry.get<TileLayer>(entity);
    ASSERT_EQ(48, tileLayer.matrix.size() * tileLayer.matrix.at(0).size());
  }

  {
    const auto& propertyEntity = context.properties.at(0);
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_EQ("a", property.name);
    ASSERT_EQ(42, property.value.as_int());
  }

  {
    const auto& propertyEntity = context.properties.at(1);
    const auto& property = registry.get<Property>(propertyEntity);
    ASSERT_EQ("b", property.name);
    ASSERT_EQ(2.5f, property.value.as_float());
  }
}

void CheckObjectLayer1(const entt::registry& registry)
{
  const auto entity = sys::FindLayer(registry, 3);
  ASSERT_NE(gNullEntity, entity);

  const auto& context = registry.get<PropertyContext>(entity);
  ASSERT_EQ("Object Layer 1", context.name);
  ASSERT_TRUE(context.properties.empty());
  ASSERT_TRUE(context.components.empty());

  const auto& layer = registry.get<Layer>(entity);
  ASSERT_EQ(LayerType::ObjectLayer, layer.type);
  ASSERT_EQ(1.0f, layer.opacity);
  ASSERT_TRUE(layer.visible);

  ASSERT_TRUE(registry.all_of<ObjectLayer>(entity));

  const auto& objectLayer = registry.get<ObjectLayer>(entity);
  ASSERT_EQ(2, objectLayer.objects.size());

  ASSERT_TRUE(sys::HasObject(registry, entity, 2));
  ASSERT_TRUE(sys::HasObject(registry, entity, 3));
  ASSERT_FALSE(sys::HasObject(registry, entity, 42));

  {
    const auto rectEntity = sys::FindObject(registry, entity, 2);
    const auto& rect = registry.get<Object>(rectEntity);
    const auto& rectContext = registry.get<PropertyContext>(rectEntity);

    ASSERT_EQ("Rect", rectContext.name);
    ASSERT_EQ(36, rect.x);
    ASSERT_EQ(40, rect.y);
    ASSERT_EQ(28, rect.width);
    ASSERT_EQ(30, rect.height);
    ASSERT_EQ("CustomType", rect.tag);
    ASSERT_TRUE(rect.visible);
    ASSERT_EQ(ObjectType::Rectangle, rect.type);
  }
}

void CheckLayers(const Document& document)
{
  const auto& registry = document.registry;
  const auto& activeLayer = registry.ctx<ActiveLayer>();

  ASSERT_EQ(3, registry.storage<Layer>().size());
  ASSERT_EQ(1, registry.get<Layer>(activeLayer.entity).id);

  CheckTileLayer1(registry);
  CheckTileLayer2(registry);
  CheckObjectLayer1(registry);
}

void CheckTilesets(const entt::registry& registry)
{
  ASSERT_EQ(2, registry.storage<Tileset>().size());

  const auto& tilesetEntity = registry.ctx<ActiveTileset>().entity;
  ASSERT_NE(gNullEntity, tilesetEntity);

  const auto& context = registry.get<PropertyContext>(tilesetEntity);
  ASSERT_EQ("outside", context.name);
  ASSERT_TRUE(context.properties.empty());
  ASSERT_TRUE(context.components.empty());

  const auto& tileset = registry.get<Tileset>(tilesetEntity);
  ASSERT_EQ(1025, tileset.first_id);
  ASSERT_EQ(32, tileset.row_count);
  ASSERT_EQ(32, tileset.column_count);
  ASSERT_EQ(1024, tileset.tile_count);

  const auto& texture = registry.get<Texture>(tilesetEntity);
  ASSERT_EQ(1024, texture.width);
  ASSERT_EQ(1024, texture.height);
}

}  // namespace

TEST(CreateDocumentFromIR, Test)
{
  MapParser parser{"test-resources/yaml/map.yaml"};
  ASSERT_EQ(IO::ParseError::None, parser.GetError());
  ASSERT_TRUE(parser);

  TextureManager textures;
  const auto document = CreateDocumentFromIR(parser.GetData(), textures);

  ASSERT_EQ(std::filesystem::absolute("test-resources/yaml/map.yaml"), document.path);
  ASSERT_FALSE(document.commands.CanUndo());
  ASSERT_FALSE(document.commands.CanRedo());

  const auto& registry = document.registry;
  CheckMapAttributes(registry);
  CheckLayers(document);
  CheckTilesets(registry);
}
