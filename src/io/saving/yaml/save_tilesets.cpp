#include "save_tilesets.hpp"

#include <format>   // format
#include <fstream>  // ofstream
#include <string>   // string

#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/property_context.hpp"
#include "core/components/texture.hpp"
#include "core/components/tileset.hpp"
#include "core/systems/tileset_system.hpp"
#include "io/saving/common_saving.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

constexpr int tileset_node_version = 1;

void SaveAnimation(YAML::Emitter& emitter,
                   const entt::registry& registry,
                   const Animation& animation)
{
  emitter << YAML::Key << "animation" << YAML::BeginSeq;

  for (const auto frameEntity : animation.frames) {
    emitter << YAML::BeginMap;

    const auto& frame = registry.get<AnimationFrame>(frameEntity);
    const auto tile = Sys::ConvertToLocal(registry, frame.tile).value();
    emitter << YAML::Key << "tile" << YAML::Value << tile;
    emitter << YAML::Key << "duration" << YAML::Value << frame.duration.count();

    emitter << YAML::EndMap;
  }

  emitter << YAML::EndSeq;
}

void SaveFancyTiles(YAML::Emitter& emitter,
                    const entt::registry& registry,
                    const Tileset& tileset,
                    const std::filesystem::path& dir)
{
  emitter << YAML::Key << "tiles" << YAML::BeginSeq;

  for (auto&& [entity, tile] : registry.view<FancyTile>().each()) {
    // TODO include this check for other emitters (JSON and XML)
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      if (IsTileWorthSaving(registry, entity)) {
        emitter << YAML::BeginMap;

        const auto id = Sys::ConvertToLocal(registry, tile.id).value();
        emitter << YAML::Key << "id" << YAML::Value << id;

        if (const auto* animation = registry.try_get<Animation>(entity)) {
          SaveAnimation(emitter, registry, *animation);
        }

        SaveProperties(emitter, registry, entity, dir);

        emitter << YAML::EndMap;
      }
    }
  }

  emitter << YAML::EndSeq;
}

void SaveTileset(const entt::registry& registry,
                 const entt::entity entity,
                 const std::string& fileName,
                 const std::filesystem::path& dir)
{
  const auto& tileset = registry.get<Tileset>(entity);
  const auto& texture = registry.get<Texture>(entity);
  const auto& ctx = registry.get<PropertyContext>(entity);

  YAML::Emitter emitter;
  emitter.SetIndent(2);

  emitter << YAML::BeginMap;
  emitter << YAML::Key << "version" << YAML::Value << tileset_node_version;
  emitter << YAML::Key << "name" << YAML::Value << ctx.name;
  emitter << YAML::Key << "tile-count" << YAML::Value << tileset.tile_count;
  emitter << YAML::Key << "tile-width" << YAML::Value << tileset.tile_width;
  emitter << YAML::Key << "tile-height" << YAML::Value << tileset.tile_height;
  emitter << YAML::Key << "column-count" << YAML::Value << tileset.column_count;
  emitter << YAML::Key << "image-path" << YAML::Value
          << GetTilesetImagePath(texture.path, dir);
  emitter << YAML::Key << "image-width" << YAML::Value << texture.width;
  emitter << YAML::Key << "image-height" << YAML::Value << texture.height;

  SaveProperties(emitter, registry, entity, dir);
  SaveFancyTiles(emitter, registry, tileset, dir);

  emitter << YAML::EndMap;

  std::ofstream stream{dir / fileName};
  stream << emitter.c_str();
}

}  // namespace

void SaveTilesets(YAML::Emitter& emitter,
                  const entt::registry& registry,
                  const std::filesystem::path& dir)
{
  if (!registry.empty<Tileset>()) {
    emitter << YAML::Key << "tilesets";
    emitter << YAML::BeginSeq;

    for (auto&& [entity, tileset, ctx] : registry.view<Tileset, PropertyContext>().each())
    {
      const auto fileName = std::format("{}.yaml", ctx.name);
      SaveTileset(registry, entity, fileName, dir);

      emitter << YAML::BeginMap;
      emitter << YAML::Key << "first-global-id" << YAML::Value << tileset.first_id;
      emitter << YAML::Key << "path" << YAML::Value << fileName;
      emitter << YAML::EndMap;
    }

    emitter << YAML::EndSeq;
  }
}

}  // namespace Tactile::IO
