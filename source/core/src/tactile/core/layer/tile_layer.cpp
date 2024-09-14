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
namespace {

void _add_n_rows(TileMatrix& matrix,
                 const std::size_t n,
                 const Extent2D::value_type column_count)
{
  matrix.reserve(matrix.size() + n);

  for (std::size_t i = 0; i < n; ++i) {
    matrix.emplace_back(column_count, kEmptyTile);
  }
}

void _remove_n_rows(TileMatrix& matrix, const std::size_t n)
{
  for (std::size_t i = 0; i < n; ++i) {
    TACTILE_ASSERT(!matrix.empty());
    matrix.pop_back();
  }
}

void _add_n_columns(TileMatrix& matrix, const std::size_t n)
{
  for (auto& row : matrix) {
    row.reserve(row.size() + n);

    for (std::size_t i = 0; i < n; ++i) {
      row.push_back(kEmptyTile);
    }
  }
}

void _remove_n_columns(TileMatrix& matrix, const std::size_t n)
{
  for (auto& row : matrix) {
    for (std::size_t i = 0; i < n; ++i) {
      TACTILE_ASSERT(!row.empty());
      row.pop_back();
    }
  }
}

void _resize(TileMatrix& matrix, const Extent2D& extent)
{
  const auto old_height = matrix.size();
  const auto old_width = old_height > 0 ? matrix.at(0).size() : std::size_t {0};

  if (old_width != extent.cols) {
    if (old_width > extent.cols) {
      _remove_n_columns(matrix, old_width - extent.cols);
    }
    else {
      _add_n_columns(matrix, extent.cols - old_width);
    }
  }

  if (old_height != extent.rows) {
    if (old_height > extent.rows) {
      _remove_n_rows(matrix, old_height - extent.rows);
    }
    else {
      _add_n_rows(matrix, extent.rows - old_height, extent.cols);
    }
  }
}

void _resize(SparseTileMatrix& matrix, const Extent2D& extent)
{
  std::erase_if(matrix, [&](const auto& position_and_tile) {
    return !extent.contains(position_and_tile.first);
  });
}

void _set_tile_unchecked(TileMatrix& matrix, const Index2D& index, const TileID tile_id)
{
  TACTILE_ASSERT(index.y < matrix.size());
  TACTILE_ASSERT(index.x < matrix.at(0).size());
  matrix[index.y][index.x] = tile_id;
}

void _set_tile_unchecked(SparseTileMatrix& matrix, const Index2D& index, const TileID tile_id)
{
  if (tile_id == kEmptyTile) {
    matrix.erase(index);
  }
  else {
    matrix.insert_or_assign(index, tile_id);
  }
}

[[nodiscard]]
auto _get_tile_unchecked(const TileMatrix& matrix, const Index2D& index) noexcept -> TileID
{
  TACTILE_ASSERT(index.y < matrix.size());
  TACTILE_ASSERT(index.x < matrix.at(0).size());
  return matrix[index.y][index.x];
}

[[nodiscard]]
auto _get_tile_unchecked(const SparseTileMatrix& matrix, const Index2D& index) -> TileID
{
  const auto iter = matrix.find(index);
  return iter != matrix.end() ? iter->second : kEmptyTile;
}

}  // namespace

auto is_tile_layer(const Registry& registry, const EntityID id) -> bool
{
  return registry.has<CMeta>(id) &&       //
         registry.has<CLayer>(id) &&      //
         registry.has<CTileLayer>(id) &&  //
         (registry.has<CDenseTileLayer>(id) || registry.has<CSparseTileLayer>(id));
}

auto make_tile_layer(Registry& registry, const Extent2D& extent) -> EntityID
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

    each_layer_tile(registry, layer_id, [&](const Index2D& index, const TileID tile_id) {
      tile_matrix[index.y][index.x] = tile_id;
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

    each_layer_tile(registry, layer_id, [&](const Index2D& index, const TileID tile_id) {
      if (tile_id != kEmptyTile) {
        sparse.tiles.insert_or_assign(index, tile_id);
      }
    });

    registry.erase<CDenseTileLayer>(layer_id);
  }

  TACTILE_ASSERT(is_tile_layer(registry, layer_id));
}

void resize_tile_layer(Registry& registry, const EntityID layer_id, const Extent2D& extent)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  auto& tile_layer = registry.get<CTileLayer>(layer_id);
  tile_layer.extent = extent;

  if (auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    _resize(dense->tiles, extent);
  }
  else if (auto* sparse = registry.find<CSparseTileLayer>(layer_id)) {
    _resize(sparse->tiles, extent);
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
  byte_stream.reserve(tile_layer.extent.rows * tile_layer.extent.cols * sizeof(TileID));

  each_layer_tile(registry, layer_id, [&](const Index2D&, const TileID tile_id) {
    each_byte(to_little_endian(tile_id),
              [&](const std::uint8_t byte) { byte_stream.push_back(byte); });
  });

  return byte_stream;
}

void set_layer_tile(Registry& registry,
                    const EntityID layer_id,
                    const Index2D& index,
                    const TileID tile_id)
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (const auto& tile_layer = registry.get<CTileLayer>(layer_id);
      !tile_layer.extent.contains(index)) {
    return;
  }

  if (auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    _set_tile_unchecked(dense->tiles, index, tile_id);
  }
  else if (auto* sparse = registry.find<CSparseTileLayer>(layer_id)) {
    _set_tile_unchecked(sparse->tiles, index, tile_id);
  }
  else {
    throw Exception {"invalid tile layer"};
  }
}

auto get_layer_tile(const Registry& registry,
                    const EntityID layer_id,
                    const Index2D& index) -> std::optional<TileID>
{
  TACTILE_ASSERT(is_tile_layer(registry, layer_id));

  if (const auto& tile_layer = registry.get<CTileLayer>(layer_id);
      !tile_layer.extent.contains(index)) {
    return std::nullopt;
  }

  if (const auto* dense = registry.find<CDenseTileLayer>(layer_id)) {
    return _get_tile_unchecked(dense->tiles, index);
  }

  if (const auto* sparse = registry.find<CSparseTileLayer>(layer_id)) {
    return _get_tile_unchecked(sparse->tiles, index);
  }

  throw Exception {"invalid tile layer"};
}

}  // namespace tactile
