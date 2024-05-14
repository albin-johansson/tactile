// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {
namespace {

template <typename TileMatrix1, typename TileMatrix2>
void _copy_tile_matrix(const TileMatrix1& from, TileMatrix2& to)
{
  const auto extent = from.get_extent();
  to.resize(extent);

  for (MatrixExtent::value_type row = 0; row < extent.rows; ++row) {
    for (MatrixExtent::value_type col = 0; col < extent.cols; ++col) {
      const MatrixIndex index {row, col};
      const auto tile_id = from.at(index);

      // Don't add empty tiles because that would bloat sparse tile matrices.
      if (tile_id != kEmptyTile) {
        to[index] = tile_id;
      }
    }
  }
}

}  // namespace

auto is_tile_layer(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&       //
         registry.has<CLayer>(entity) &&      //
         registry.has<CTileLayer>(entity) &&  //
         (registry.has<CDenseTileLayer>(entity) ||
          registry.has<CSparseTileLayer>(entity));
}

auto make_tile_layer(Registry& registry, const MatrixExtent& extent) -> EntityID
{
  const auto entity = make_unspecialized_layer(registry);

  registry.add<CTileLayer>(entity);

  auto& dense = registry.add<CDenseTileLayer>(entity);
  dense.tiles.resize(extent);

  TACTILE_ASSERT(is_tile_layer(registry, entity));
  return entity;
}

void destroy_tile_layer(Registry& registry, const EntityID layer_entity)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
  registry.destroy(layer_entity);
}

void to_dense_tile_layer(Registry& registry, const EntityID layer_entity)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));

  const auto sparse_tile_layer =
      registry.detach<CSparseTileLayer>(layer_entity);

  if (sparse_tile_layer.has_value()) {
    auto& dense_tile_layer = registry.add<CDenseTileLayer>(layer_entity);
    _copy_tile_matrix(sparse_tile_layer->tiles, dense_tile_layer.tiles);
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
}

void to_sparse_tile_layer(Registry& registry, const EntityID layer_entity)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));

  const auto dense_tile_layer = registry.detach<CDenseTileLayer>(layer_entity);

  if (dense_tile_layer.has_value()) {
    auto& sparse_tile_layer = registry.add<CSparseTileLayer>(layer_entity);
    _copy_tile_matrix(dense_tile_layer->tiles, sparse_tile_layer.tiles);
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
}

}  // namespace tactile
