// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include "tactile/core/debug/assert.hpp"
#include "tactile/core/entity/registry.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {
inline namespace tile_layer {

void _copy_tile_matrix(const ITileMatrix& from, ITileMatrix& to)
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

}  // namespace tile_layer

auto is_tile_layer(const Registry& registry, const EntityID entity) -> bool
{
  return registry.has<CMeta>(entity) &&   //
         registry.has<CLayer>(entity) &&  //
         registry.has<CTileLayer>(entity);
}

auto make_tile_layer(Registry& registry, const MatrixExtent& extent) -> EntityID
{
  const auto entity = make_unspecialized_layer(registry);

  auto& tile_layer = registry.add<CTileLayer>(entity);
  auto& tiles = tile_layer.tiles.emplace<DenseTileMatrix>();
  tiles.resize(extent);

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
  auto& tile_layer = registry.get<CTileLayer>(layer_entity);

  if (const auto* sparse_tiles =
          std::get_if<SparseTileMatrix>(&tile_layer.tiles)) {
    DenseTileMatrix dense_tiles {};
    _copy_tile_matrix(*sparse_tiles, dense_tiles);
    tile_layer.tiles.emplace<DenseTileMatrix>(std::move(dense_tiles));
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
}

void to_sparse_tile_layer(Registry& registry, const EntityID layer_entity)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
  auto& tile_layer = registry.get<CTileLayer>(layer_entity);

  if (const auto* dense_tiles =
          std::get_if<DenseTileMatrix>(&tile_layer.tiles)) {
    SparseTileMatrix sparse_tiles {};
    _copy_tile_matrix(*dense_tiles, sparse_tiles);
    tile_layer.tiles.emplace<SparseTileMatrix>(std::move(sparse_tiles));
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
}

auto get_tile_layer_data(Registry& registry,
                         const EntityID layer_entity) -> ITileMatrix&
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
  auto& tile_layer = registry.get<CTileLayer>(layer_entity);

  if (std::holds_alternative<DenseTileMatrix>(tile_layer.tiles)) {
    return std::get<DenseTileMatrix>(tile_layer.tiles);
  }

  TACTILE_ASSERT(std::holds_alternative<SparseTileMatrix>(tile_layer.tiles));
  return std::get<SparseTileMatrix>(tile_layer.tiles);
}

auto get_tile_layer_data(const Registry& registry,
                         const EntityID layer_entity) -> const ITileMatrix&
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_entity));
  const auto& tile_layer = registry.get<CTileLayer>(layer_entity);

  if (std::holds_alternative<DenseTileMatrix>(tile_layer.tiles)) {
    return std::get<DenseTileMatrix>(tile_layer.tiles);
  }

  TACTILE_ASSERT(holds_alternative<SparseTileMatrix>(tile_layer.tiles));
  return std::get<SparseTileMatrix>(tile_layer.tiles);
}

}  // namespace tactile
