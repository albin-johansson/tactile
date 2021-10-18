#include "append_fancy_tiles.hpp"

#include "append_properties.hpp"
#include "tactile/core/components/animation.hpp"
#include "tactile/core/components/fancy_tile.hpp"
#include "tactile/core/systems/tileset_system.hpp"
#include "tactile/io/saving/common_saving.hpp"

namespace Tactile::IO {
namespace {

void AppendFrame(pugi::xml_node animationNode,
                 const entt::registry& registry,
                 const AnimationFrame& frame)
{
  auto frameNode = animationNode.append_child("frame");

  const auto local = Sys::ConvertToLocal(registry, frame.tile).value();
  frameNode.append_attribute("tileid").set_value(local);
  frameNode.append_attribute("duration").set_value(frame.duration.count());
}

void AppendAnimation(pugi::xml_node tileNode,
                     const entt::registry& registry,
                     const Animation& animation)
{
  auto animationNode = tileNode.append_child("animation");
  for (const auto frameEntity : animation.frames) {
    const auto& frame = registry.get<AnimationFrame>(frameEntity);
    AppendFrame(animationNode, registry, frame);
  }
}

}  // namespace

void AppendFancyTiles(pugi::xml_node node,
                      const entt::registry& registry,
                      const Tileset& tileset,
                      const std::filesystem::path& dir)
{
  for (auto&& [entity, tile] : registry.view<FancyTile>().each()) {
    if (tile.id >= tileset.first_id && tile.id <= tileset.last_id) {
      if (IsTileWorthSaving(registry, entity)) {
        auto tileNode = node.append_child("tile");

        const auto id = Sys::ConvertToLocal(registry, tile.id).value();
        tileNode.append_attribute("id").set_value(id);

        if (const auto* animation = registry.try_get<Animation>(entity)) {
          AppendAnimation(tileNode, registry, *animation);
        }

        AppendProperties(registry, entity, tileNode, dir);
      }
    }
  }
}

}  // namespace Tactile::IO
