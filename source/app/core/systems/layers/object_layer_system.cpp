#include "object_layer_system.hpp"

#include "core/components/object.hpp"
#include "core/map.hpp"
#include "core/systems/viewport_system.hpp"
#include "layer_system.hpp"
#include "throw.hpp"

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
  if (object.type == object_type::point) {
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

auto get_object_layer(const entt::registry& registry, const layer_id id)
    -> std::pair<entt::entity, const comp::object_layer&>
{
  const auto entity = find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::object_layer>(entity)) {
    return {entity, registry.get<comp::object_layer>(entity)};
  }
  else {
    throw_traced(tactile_error{"Invalid object layer ID!"});
  }
}

auto has_object(const entt::registry& registry,
                const comp::object_layer& layer,
                const object_id id) -> bool
{
  return find_object(registry, layer, id) != entt::null;
}

auto find_object(const entt::registry& registry,
                 const comp::object_layer& layer,
                 const object_id id) -> entt::entity
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
