// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile.hpp"

#include "tactile/base/io/save/ir.hpp"
#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/meta/meta.hpp"
#include "tactile/core/tile/animation.hpp"

namespace tactile {
namespace tile {

[[nodiscard]]
auto convert_animation(Registry& registry, const EntityID tile_id, const ir::Tile& ir_tile)
    -> std::expected<void, ErrorCode>
{
  std::size_t frame_index = 0;

  for (const auto& ir_frame : ir_tile.animation) {
    const AnimationFrame frame {ir_frame.tile_index, ir_frame.duration};

    const auto add_frame_result = add_animation_frame(registry, tile_id, frame_index, frame);
    if (!add_frame_result.has_value()) {
      return std::unexpected {add_frame_result.error()};
    }

    ++frame_index;
  }

  return {};
}

}  // namespace tile

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
  const auto tile_id = make_tile(registry, ir_tile.index);

  const auto convert_animation_result = tile::convert_animation(registry, tile_id, ir_tile);
  if (!convert_animation_result.has_value()) {
    destroy_tile(registry, tile_id);
    return std::unexpected {convert_animation_result.error()};
  }

  auto& tile = registry.get<CTile>(tile_id);
  tile.objects.reserve(ir_tile.objects.size());

  for (const auto& ir_object : ir_tile.objects) {
    tile.objects.push_back(make_object(registry, ir_object));
  }

  convert_ir_metadata(registry, tile_id, ir_tile.meta);

  TACTILE_ASSERT(is_tile(registry, tile_id));
  return tile_id;
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

  const auto copy_entity = registry.make_entity();
  registry.add<CMeta>(copy_entity, meta);

  auto& tile_copy = registry.add<CTile>(copy_entity);
  tile_copy.index = tile.index;

  tile_copy.objects.reserve(tile_copy.objects.size());
  for (const auto object_entity : tile.objects) {
    tile_copy.objects.push_back(copy_object(registry, object_entity));
  }

  TACTILE_ASSERT(is_tile(registry, copy_entity));
  return copy_entity;
}

auto is_tile_plain(const Registry& registry, const EntityID tile_id) -> bool
{
  TACTILE_ASSERT(is_tile(registry, tile_id));

  const auto& tile = registry.get<CTile>(tile_id);
  const auto& meta = registry.get<CMeta>(tile_id);

  return !registry.has<CAnimation>(tile_id) &&  //
         tile.objects.empty() &&                //
         meta.properties.empty() &&             //
         meta.components.empty();
}

}  // namespace tactile
