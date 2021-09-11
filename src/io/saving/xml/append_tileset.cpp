#include "append_tileset.hpp"

#include <format>  // format

#include "append_properties.hpp"
#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/property_context.hpp"
#include "core/components/texture.hpp"
#include "core/systems/tileset_system.hpp"
#include "io/preferences.hpp"
#include "io/saving/common_saving.hpp"
#include "save_xml.hpp"

namespace Tactile::IO {
namespace {

void AppendFancyTiles(pugi::xml_node node,
                      const entt::registry& registry,
                      const entt::entity tilesetEntity,
                      const std::filesystem::path& dir)
{
  for (auto&& [entity, tile] : registry.view<FancyTile>().each())
  {
    auto tileNode = node.append_child("tile");
    tileNode.append_attribute("id").set_value(
        Sys::ConvertToLocal(registry, tile.id).value());

    if (const auto* animation = registry.try_get<Animation>(entity))
    {
      auto animationNode = tileNode.append_child("animation");
      for (const auto frameEntity : animation->frames)
      {
        const auto& frame = registry.get<AnimationFrame>(frameEntity);
        auto frameNode = animationNode.append_child("frame");

        const auto local = Sys::ConvertToLocal(registry, frame.tile).value();
        frameNode.append_attribute("tileid").set_value(local);
        frameNode.append_attribute("duration").set_value(frame.duration.count());
      }
    }

    AppendProperties(registry, entity, tileNode, dir);
  }
}

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

  AppendFancyTiles(node, registry, tilesetEntity, dir);
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
  if (Prefs::GetEmbedTilesets())
  {
    AppendEmbeddedTileset(mapNode, registry, tilesetEntity, dir);
  }
  else
  {
    const auto& context = registry.get<PropertyContext>(tilesetEntity);
    const auto source = std::format("{}.tsx", context.name);
    CreateExternalTilesetFile(registry, tilesetEntity, source, dir);
    AppendExternalTileset(mapNode, registry, tilesetEntity, source);
  }
}

}  // namespace Tactile::IO
