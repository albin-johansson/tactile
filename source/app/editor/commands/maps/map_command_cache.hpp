#pragma once

#include <entt/entt.hpp>  // registry

#include "core/tile_position.hpp"
#include "tactile.hpp"

namespace tactile {

class map_command_cache final
{
  using tile_cache = tree_map<tile_position, tile_id>;
  using layer_cache = tree_map<layer_id, tile_cache>;

 public:
  void clear() noexcept;

  void restore_tiles(entt::registry& registry);

  void save_tiles(const entt::registry& registry,
                  const tile_position& begin,
                  const tile_position& end);

  void merge_with(const map_command_cache& other);

 private:
  layer_cache mCache;
};

}  // namespace tactile
