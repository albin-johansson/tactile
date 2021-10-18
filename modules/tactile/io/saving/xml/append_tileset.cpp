#include "append_tileset.hpp"

#include <format>  // format

#include "append_fancy_tiles.hpp"
#include "append_properties.hpp"
#include "save_xml.hpp"
#include "tactile/core/components/property_context.hpp"
#include "tactile/core/components/texture.hpp"
#include "tactile/core/systems/tileset_system.hpp"
#include "tactile/io/preferences.hpp"
#include "tactile/io/saving/common_saving.hpp"

namespace Tactile::IO {
namespace {

void AddCommon(pugi::xml_node node,
               const entt::registry& registry,
               const entt::entity tilesetEntity,
               const std::filesystem::path& dir)
{
  const auto& tileset = registry.get<Tileset>(tilesetEntity);
  const auto& texture = registry.get<Texture>(tilesetEntity);
  const auto& context = registry.get<PropertyContext>(tilesetEntity);

  node.append_attribute("name").set_value(context.name.c_str());
  node.append_attribute("tilewidth").set_value(tileset.tile_width);
  node.append_attribute("tileheight").set_value(tileset.tile_height);
  node.append_attribute("tilecount").set_value(tileset.tile_count);
  node.append_attribute("columns").set_value(tileset.column_count);

  {
    auto imageNode = node.append_child("image");
    const auto source = GetTilesetImagePath(texture.path, dir);
    imageNode.append_attribute("source").set_value(source.c_str());
    imageNode.append_attribute("width").set_value(texture.width);
    imageNode.append_attribute("height").set_value(texture.height);
  }

  AppendFancyTiles(node, registry, tileset, dir);
  AppendProperties(registry, tilesetEntity, node, dir);
}

void AppendEmbeddedTileset(pugi::xml_node mapNode,
                           const entt::registry& registry,
                           const entt::entity tilesetEntity,
                           const std::filesystem::path& dir)
{
  const auto& tileset = registry.get<Tileset>(tilesetEntity);

  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset.first_id.get());

  AddCommon(node, registry, tilesetEntity, dir);
}

void CreateExternalTilesetFile(const entt::registry& registry,
                               const entt::entity tilesetEntity,
                               const std::string& source,
                               const std::filesystem::path& dir)
{
  pugi::xml_document xml;

  auto root = xml.append_child("tileset");
  root.append_attribute("version").set_value(tiled_xml_version);
  root.append_attribute("tiledversion").set_value(tiled_version);

  AddCommon(root, registry, tilesetEntity, dir);

  const auto path = dir / source;
  CENTURION_LOG_INFO("Saving external tileset in \"%s\"", path.string().c_str());

  SaveXml(xml, path);
}

void AppendExternalTileset(pugi::xml_node mapNode,
                           const entt::registry& registry,
                           const entt::entity tilesetEntity,
                           const std::string& source)
{
  const auto& tileset = registry.get<Tileset>(tilesetEntity);

  auto node = mapNode.append_child("tileset");
  node.append_attribute("firstgid").set_value(tileset.first_id);
  node.append_attribute("source").set_value(source.c_str());
}

}  // namespace

void AppendTileset(pugi::xml_node mapNode,
                   const entt::registry& registry,
                   const entt::entity tilesetEntity,
                   const std::filesystem::path& dir)
{
  if (Prefs::GetEmbedTilesets()) {
    AppendEmbeddedTileset(mapNode, registry, tilesetEntity, dir);
  }
  else {
    const auto& context = registry.get<PropertyContext>(tilesetEntity);
    const auto source = std::format("{}.tsx", context.name);
    CreateExternalTilesetFile(registry, tilesetEntity, source, dir);
    AppendExternalTileset(mapNode, registry, tilesetEntity, source);
  }
}

}  // namespace Tactile::IO
