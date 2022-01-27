#include "object_layer_system.hpp"

#include "core/components/object.hpp"
#include "core/map.hpp"
#include "core/systems/viewport_system.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto _get_hit_detection_bounds(const comp::object& object,
                                             const float mapTileWidth,
                                             const float mapTileHeight,
                                             const float xRatio,
                                             const float yRatio) -> cen::frect
{
  /* Points have no width or height, so we have to create a hitbox large enough to be easy
     for the user to click */
  if (object.type == ObjectType::Point) {
    const auto width = static_cast<float>(mapTileWidth) / 2.0f;
    const auto height = static_cast<float>(mapTileHeight) / 2.0f;

    return {(object.x - (width / 2.0f)) * xRatio,
            (object.y - (height / 2.0f)) * yRatio,
            width * xRatio,
            height * yRatio};
  }
  else {
    return {object.x * xRatio,
            object.y * yRatio,
            object.width * xRatio,
            object.height * yRatio};
  }
}

}  // namespace

auto has_object(const entt::registry& registry,
                const comp::object_layer& layer,
                const ObjectID id) -> bool
{
  return find_object(registry, layer, id) != entt::null;
}

auto find_object(const entt::registry& registry,
                 const comp::object_layer& layer,
                 const ObjectID id) -> entt::entity
{
  for (const auto objectEntity : layer.objects) {
    const auto& object = registry.get<comp::object>(objectEntity);
    if (object.id == id) {
      return objectEntity;
    }
  }

  return entt::null;
}

auto find_object(const entt::registry& registry,
                 const comp::object_layer& layer,
                 const float x,
                 const float y) -> entt::entity
{
  const auto& map = registry.ctx<MapInfo>();
  const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);

  for (const auto objectEntity : layer.objects) {
    const auto& object = registry.get<comp::object>(objectEntity);
    const auto bounds = _get_hit_detection_bounds(object,
                                                  static_cast<float>(map.tile_width),
                                                  static_cast<float>(map.tile_height),
                                                  xRatio,
                                                  yRatio);
    if (bounds.contains({x, y})) {
      return objectEntity;
    }
  }

  return entt::null;
}

}  // namespace tactile::sys
