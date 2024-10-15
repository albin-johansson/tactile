// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile.hpp"

#include <cstddef>  // size_t

#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/animation.hpp"
#include "tactile/core/tile/tile_types.hpp"

namespace tactile::core {
namespace {

[[nodiscard]]
auto _restore_animation(Registry& registry,
                        const EntityID tile_entity,
                        const ir::Tile& ir_tile) -> std::expected<void, ErrorCode>
{
  std::size_t frame_index = 0;

  for (const auto& ir_frame : ir_tile.animation) {
    const AnimationFrame frame {ir_frame.tile_index, ir_frame.duration};

    const auto add_frame_result =
        add_animation_frame(registry, tile_entity, frame_index, frame);
    if (!add_frame_result.has_value()) {
      return std::unexpected {add_frame_result.error()};
    }

    ++frame_index;
  }

  return {};
}

}  // namespace

auto is_tile(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) && registry.has<CTile>(entity);
}

auto make_tile(Registry& registry, const TileIndex index) -> EntityID
{
  const auto tile_entity = registry.make_entity();
  registry.add<CMeta>(tile_entity);

  auto& tile = registry.add<CTile>(tile_entity);
  tile.index = index;

  TACTILE_ASSERT(is_tile(registry, tile_entity));
  return tile_entity;
}

auto make_tile(Registry& registry, const ir::Tile& ir_tile)
    -> std::expected<EntityID, ErrorCode>
{
  const auto tile_entity = make_tile(registry, ir_tile.index);

  const auto restore_animation_result = _restore_animation(registry, tile_entity, ir_tile);
  if (!restore_animation_result.has_value()) {
    destroy_tile(registry, tile_entity);
    return std::unexpected {restore_animation_result.error()};
  }

  auto& tile = registry.get<CTile>(tile_entity);
  tile.objects.reserve(ir_tile.objects.size());

  for (const auto& ir_object : ir_tile.objects) {
    tile.objects.push_back(make_object(registry, ir_object));
  }

  convert_ir_metadata(registry, tile_entity, ir_tile.meta);

  TACTILE_ASSERT(is_tile(registry, tile_entity));
  return tile_entity;
}

void destroy_tile(Registry& registry, const EntityID tile_entity)
{
  TACTILE_ASSERT(is_tile(registry, tile_entity));
  const auto& tile = registry.get<CTile>(tile_entity);

  for (const auto object_entity : tile.objects) {
    destroy_object(registry, object_entity);
  }

  registry.destroy(tile_entity);
}

auto copy_tile(Registry& registry, const EntityID tile_entity) -> EntityID
{
  TACTILE_ASSERT(is_tile(registry, tile_entity));
  const auto& meta = registry.get<CMeta>(tile_entity);
  const auto& tile = registry.get<CTile>(tile_entity);

  const auto new_tile_entity = registry.make_entity();
  registry.add<CMeta>(new_tile_entity, meta);

  auto& new_tile = registry.add<CTile>(new_tile_entity);
  new_tile.index = tile.index;

  new_tile.objects.reserve(new_tile.objects.size());
  for (const auto object_entity : tile.objects) {
    new_tile.objects.push_back(copy_object(registry, object_entity));
  }

  TACTILE_ASSERT(is_tile(registry, new_tile_entity));
  return new_tile_entity;
}

auto is_tile_plain(const Registry& registry, const EntityID tile_entity) -> bool
{
  TACTILE_ASSERT(is_tile(registry, tile_entity));

  const auto& tile = registry.get<CTile>(tile_entity);
  const auto& meta = registry.get<CMeta>(tile_entity);

  return !registry.has<CAnimation>(tile_entity) &&  //
         tile.objects.empty() &&                    //
         meta.properties.empty() &&                 //
         meta.components.empty();
}

}  // namespace tactile::core
