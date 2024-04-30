// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tile/tile.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/object.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {

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

}  // namespace tactile
