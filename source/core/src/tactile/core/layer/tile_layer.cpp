// Copyright (C) 2024 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/layer/tile_layer.hpp"

#include <utility>  // move

#include "tactile/base/io/tile_io.hpp"
#include "tactile/base/numeric/saturate_cast.hpp"
#include "tactile/base/platform/bits.hpp"
#include "tactile/base/util/tile_matrix.hpp"
#include "tactile/core/layer/layer.hpp"
#include "tactile/core/meta/meta.hpp"

namespace tactile {
namespace tile_layer {

void add_n_rows(TileMatrix& matrix,
                const std::ptrdiff_t n,
                const MatrixExtent::value_type column_count)
{
  matrix.reserve(matrix.size() + saturate_cast<std::size_t>(n));

  for (std::ptrdiff_t i = 0; i < n; ++i) {
    matrix.emplace_back(column_count, kEmptyTile);
  }
}

void remove_n_rows(TileMatrix& matrix, const std::ptrdiff_t n)
{
  for (std::ptrdiff_t i = 0; i < n; ++i) {
    TACTILE_ASSERT(!matrix.empty());
    matrix.pop_back();
  }
}

void add_n_columns(TileMatrix& matrix, const std::ptrdiff_t n)
{
  for (auto& row : matrix) {
    row.reserve(row.size() + saturate_cast<std::size_t>(n));

    for (std::ptrdiff_t i = 0; i < n; ++i) {
      row.push_back(kEmptyTile);
    }
  }
}

void remove_n_columns(TileMatrix& matrix, const std::ptrdiff_t n)
{
  for (auto& row : matrix) {
    for (std::ptrdiff_t i = 0; i < n; ++i) {
      TACTILE_ASSERT(!row.empty());
      row.pop_back();
    }
  }
}

void resize(TileMatrix& matrix, const MatrixExtent& extent)
{
  const auto old_rows = std::ssize(matrix);
  const auto old_cols = (old_rows > 0) ? std::ssize(matrix.at(0)) : std::ptrdiff_t {0};

  if (old_cols != extent.cols) {
    if (old_cols > extent.cols) {
      remove_n_columns(matrix, old_cols - extent.cols);
    }
    else {
      add_n_columns(matrix, extent.cols - old_cols);
    }
  }

  if (old_rows != extent.rows) {
    if (old_rows > extent.rows) {
      remove_n_rows(matrix, old_rows - extent.rows);
    }
    else {
      add_n_rows(matrix, extent.rows - old_rows, extent.cols);
    }
  }
}

void resize(SparseTileMatrix& matrix, const MatrixExtent& extent)
{
  std::erase_if(matrix, [&](const auto& position_and_tile) {
    return !is_index_within_extent(extent, position_and_tile.first);
  });
}

void set_tile_unchecked(TileMatrix& matrix, const MatrixIndex& index, const TileID tile_id)
{
  TACTILE_ASSERT(index.row < std::ssize(matrix));
  TACTILE_ASSERT(index.col < std::ssize(matrix.at(0)));
  matrix[static_cast<std::size_t>(index.row)][static_cast<std::size_t>(index.col)] = tile_id;
}

void set_tile_unchecked(SparseTileMatrix& matrix,
                        const MatrixIndex& index,
                        const TileID tile_id)
{
  if (tile_id == kEmptyTile) {
    matrix.erase(index);
  }
  else {
    matrix.insert_or_assign(index, tile_id);
  }
}

[[nodiscard]]
auto get_tile_unchecked(const TileMatrix& matrix, const MatrixIndex& index) noexcept -> TileID
{
  return matrix[static_cast<std::size_t>(index.row)][static_cast<std::size_t>(index.col)];
}

[[nodiscard]]
auto get_tile_unchecked(const SparseTileMatrix& matrix, const MatrixIndex& index) -> TileID
{
  const auto iter = matrix.find(index);
  return iter != matrix.end() ? iter->second : kEmptyTile;
}

}  // namespace tile_layer

auto is_tile_layer(const Registry& registry, const EntityID id) -> bool
{
  return registry.has<CMeta>(id) &&       //
         registry.has<CLayer>(id) &&      //
         registry.has<CTileLayer>(id) &&  //
         (registry.has<CDenseTileLayer>(id) || registry.has<CSparseTileLayer>(id));
}

auto make_tile_layer(Registry& registry, const MatrixExtent& extent) -> EntityID
{
  const auto layer_id = make_unspecialized_layer(registry);

  registry.add<CTileLayer>(layer_id, extent);

  auto& dense = registry.add<CDenseTileLayer>(layer_id);
  dense.tiles = make_tile_matrix(extent);

  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
  return layer_id;
}

void destroy_tile_layer(Registry& registry, const EntityID layer_id)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
  registry.destroy(layer_id);
}

void convert_to_dense_tile_layer(Registry& registry, const EntityID layer_id)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
  const auto& tile_layer = registry.get<CTileLayer>(layer_id);

  if (registry.has<CSparseTileLayer>(layer_id)) {
    auto tile_matrix = make_tile_matrix(tile_layer.extent);

    each_layer_tile(registry, layer_id, [&](const MatrixIndex& index, const TileID tile_id) {
      tile_matrix[static_cast<std::size_t>(index.row)][static_cast<std::size_t>(index.col)] =
          tile_id;
    });

    auto& dense = registry.add<CDenseTileLayer>(layer_id);
    dense.tiles = std::move(tile_matrix);

    registry.erase<CSparseTileLayer>(layer_id);
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
}

void convert_to_sparse_tile_layer(Registry& registry, const EntityID layer_id)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (registry.has<CDenseTileLayer>(layer_id)) {
    auto& sparse = registry.add<CSparseTileLayer>(layer_id);

    each_layer_tile(registry, layer_id, [&](const MatrixIndex& index, const TileID tile_id) {
      if (tile_id != kEmptyTile) {
        sparse.tiles.insert_or_assign(index, tile_id);
      }
    });

    registry.erase<CDenseTileLayer>(layer_id);
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
}

void resize_tile_layer(Registry& registry, const EntityID layer_id, const MatrixExtent& extent)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (extent.rows < 0 || extent.cols < 0) {
    throw Exception {"invalid tile layer extent"};
  }

  auto& tile_layer = registry.get<CTileLayer>(layer_id);
  tile_layer.extent = extent;

  if (auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    tile_layer::resize(dense->tiles, extent);
  }
  else if (auto* sparse = registry.find<CSparseTileLayer>(layer_id)) {
    tile_layer::resize(sparse->tiles, extent);
  }
  else {
    throw Exception {"invalid tile layer"};
  }
}

auto serialize_tile_layer(const Registry& registry, const EntityID layer_id) -> ByteStream
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
  const auto& tile_layer = registry.get<CTileLayer>(layer_id);

  ByteStream byte_stream {};

  const auto u_rows = saturate_cast<std::size_t>(tile_layer.extent.rows);
  const auto u_cols = saturate_cast<std::size_t>(tile_layer.extent.cols);
  byte_stream.reserve(u_rows * u_cols * sizeof(TileID));

  each_layer_tile(registry, layer_id, [&](const MatrixIndex&, const TileID tile_id) {
    each_byte(to_little_endian(tile_id),
              [&](const std::uint8_t byte) { byte_stream.push_back(byte); });
  });

  return byte_stream;
}

void set_layer_tile(Registry& registry,
                    const EntityID layer_id,
                    const MatrixIndex& index,
                    const TileID tile_id)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (const auto& tile_layer = registry.get<CTileLayer>(layer_id);
      !is_index_within_extent(tile_layer.extent, index)) {
    return;
  }

  if (auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    tile_layer::set_tile_unchecked(dense->tiles, index, tile_id);
  }
  else if (auto* sparse = registry.find<CSparseTileLayer>(layer_id)) {
    tile_layer::set_tile_unchecked(sparse->tiles, index, tile_id);
  }
  else {
    throw Exception {"invalid tile layer"};
  }
}

auto get_layer_tile(const Registry& registry,
                    const EntityID layer_id,
                    const MatrixIndex& index) -> std::optional<TileID>
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (const auto& tile_layer = registry.get<CTileLayer>(layer_id);
      !is_index_within_extent(tile_layer.extent, index)) {
    return std::nullopt;
  }

  if (const auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    return tile_layer::get_tile_unchecked(dense->tiles, index);
  }

  if (const auto* sparse = registry.find<CSparseTileLayer>(layer_id)) {
    return tile_layer::get_tile_unchecked(sparse->tiles, index);
  }

  throw Exception {"invalid tile layer"};
}

auto is_index_within_extent(const MatrixExtent& extent,
                            const MatrixIndex& index) noexcept -> bool
{
  return index.row >= 0 &&           //
         index.col >= 0 &&           //
         index.row < extent.rows &&  //
         index.col < extent.cols;
}

}  // namespace tactile
