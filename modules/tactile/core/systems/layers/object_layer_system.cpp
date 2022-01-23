#include "object_layer_system.hpp"

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/components/object.hpp"
#include "core/map.hpp"
#include "core/systems/viewport_system.hpp"
#include "core/viewport.hpp"

namespace tactile::Sys {
namespace {

[[nodiscard]] auto GetHitDetectionBounds(const Object& object,
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

auto HasObject(const entt::registry& registry,
               const entt::entity entity,
               const ObjectID id) -> bool
{
  return FindObject(registry, entity, id) != entt::null;
}

auto FindObject(const entt::registry& registry,
                const entt::entity entity,
                const ObjectID id) -> entt::entity
{
  const auto& layer = registry.get<ObjectLayer>(entity);

  for (const auto objectEntity : layer.objects) {
    const auto& object = registry.get<Object>(objectEntity);
    if (object.id == id) {
      return objectEntity;
    }
  }

  return entt::null;
}

auto FindObject(const entt::registry& registry,
                const entt::entity entity,
                const float x,
                const float y) -> entt::entity
{
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<ObjectLayer>(entity));

  const auto& map = registry.ctx<Map>();
  const auto [xRatio, yRatio] = GetViewportScalingRatio(registry);

  const auto& layer = registry.get<ObjectLayer>(entity);
  for (const auto objectEntity : layer.objects) {
    const auto& object = registry.get<Object>(objectEntity);
    const auto bounds = GetHitDetectionBounds(object,
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

}  // namespace tactile::Sys
