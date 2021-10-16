#include "save_fancy_tiles.hpp"

#include "core/components/animation.hpp"
#include "core/components/fancy_tile.hpp"
#include "core/components/property_context.hpp"
#include "core/systems/tileset_system.hpp"
#include "io/saving/common_saving.hpp"
#include "save_properties.hpp"

namespace Tactile::IO {
namespace {

[[nodiscard]] auto SaveFrame(const entt::registry& registry, const AnimationFrame& frame)
    -> JSON
{
  auto json = JSON::object();

  json["tileid"] = Sys::ConvertToLocal(registry, frame.tile).value().get();
  json["duration"] = frame.duration.count();

  return json;
}

[[nodiscard]] auto SaveAnimation(const entt::registry& registry,
                                 const Animation& animation) -> JSON
{
  auto array = JSON::array();

  for (const auto entity : animation.frames) {
    const auto& frame = registry.get<AnimationFrame>(entity);
    array += SaveFrame(registry, frame);
  }

  return array;
}

[[nodiscard]] auto SaveFancyTile(const entt::registry& registry,
                                 const entt::entity entity,
                                 const FancyTile& tile,
                                 const std::filesystem::path& dir) -> JSON
{
  auto json = JSON::object();
  json["id"] = Sys::ConvertToLocal(registry, tile.id).value().get();

  if (const auto* animation = registry.try_get<Animation>(entity)) {
    json["animation"] = SaveAnimation(registry, *animation);
  }

  const auto& ctx = registry.get<PropertyContext>(entity);
  if (!ctx.properties.empty()) {
    json["properties"] = SaveProperties(registry, entity, dir);
  }

  return json;
}

}  // namespace

auto SaveFancyTiles(const entt::registry& registry,
                    const Tileset& tileset,
                    const std::filesystem::path& dir) -> JSON
{
  auto array = JSON::array();

  for (auto&& [entity, tile] : registry.view<FancyTile>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      if (IsTileWorthSaving(registry, entity)) {
        array += SaveFancyTile(registry, entity, tile, dir);
      }
    }
  }

  return array;
}

}  // namespace Tactile::IO
