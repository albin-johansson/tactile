#include "tile_layer_system.hpp"

#include "layer_system.hpp"
#include "throw.hpp"

namespace tactile::sys {
namespace {

[[nodiscard]] auto is_valid_position(const comp::tile_layer& layer,
                                     const usize row,
                                     const usize col) -> bool
{
  return row < layer.matrix.size() && col < layer.matrix[0].size();
}

}  // namespace

auto get_tile_layer_entity(const entt::registry& registry, const LayerID id)
    -> entt::entity
{
  const auto entity = sys::find_layer(registry, id);
  if (entity != entt::null && registry.all_of<comp::tile_layer>(entity)) {
    return entity;
  }
  else {
    ThrowTraced(TactileError{"Invalid tile layer identifier!"});
  }
}

void set_tile(comp::tile_layer& layer, const tile_position& position, const TileID tile)
{
  const auto row = position.row_index();
  const auto col = position.col_index();

  if (is_valid_position(layer, row, col)) {
    layer.matrix[row][col] = tile;
  }
  else {
    ThrowTraced(TactileError{"Invalid tile layer position!"});
  }
}

void set_tiles(comp::tile_layer& layer, const TileCache& tiles)
{
  for (const auto& [position, tile] : tiles) {
    const auto row = position.row_index();
    const auto col = position.col_index();
    TACTILE_ASSERT(is_valid_position(layer, row, col));
    layer.matrix[row][col] = tile;
  }
}

auto get_tile(const comp::tile_layer& layer, const tile_position& position) -> TileID
{
  const auto row = position.row_index();
  const auto col = position.col_index();

  if (is_valid_position(layer, row, col)) {
    return layer.matrix[row][col];
  }
  else {
    return empty_tile;
  }
}

}  // namespace tactile::sys
