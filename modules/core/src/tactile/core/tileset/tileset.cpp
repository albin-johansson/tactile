// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tileset.hpp"

#include "tactile/foundation/debug/assert.hpp"
#include "tactile/foundation/debug/exception.hpp"
#include "tactile/foundation/debug/validation.hpp"
#include "tactile/foundation/misc/integer_conversion.hpp"

namespace tactile::core {

Tileset::Tileset(const TilesetCreateInfo& info)
  : mTileCount {info.tile_count},
    mColumnCount {info.column_count},
    mTileSize {info.tile_size},
    mTextureSize {info.texture_size},
    mNormalizedTileSize {vector_cast<Float2::value_type>(info.tile_size) /
                         vector_cast<Float2::value_type>(info.texture_size)},
    mTextureUuid {info.texture_uuid}
{
  const auto tile_count = as_unsigned(mTileCount);

  mTiles.reserve(tile_count);
  mAppearanceCache.resize(tile_count);

  _create_tiles();
}

void Tileset::_create_tiles()
{
  for (ssize tile_index = 0; tile_index < mTileCount; ++tile_index) {
    const auto tile_row = narrow<int32>(tile_index / mColumnCount);
    const auto tile_col = narrow<int32>(tile_index % mColumnCount);

    const Rectangle tile_region {
      .position = {tile_col * mTileSize.x(), tile_row * mTileSize.y()},
      .size = mTileSize,
    };

    mTiles.emplace_back(tile_region);
  }
}

void Tileset::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);

  for (auto& tile : mTiles) {
    tile.accept(visitor);
  }
}

void Tileset::update()
{
  mAppearanceCache.assign(mAppearanceCache.size(), kNothing);

  for (auto& tile : mTiles) {
    if (tile.is_animated()) {
      auto& tile_animation = tile.get_animation();
      tile_animation.update();
    }
  }
}

auto Tileset::index_of(const TilePos& pos) const -> Maybe<TileIndex>
{
  if (pos.row >= 0 && pos.col >= 0 && pos.col < mColumnCount) {
    if (pos.row < row_count()) {
      return TileIndex {narrow<TileIndex::value_type>(pos.row * mColumnCount + pos.col)};
    }
  }

  return kNothing;
}

auto Tileset::get_appearance(const TileIndex tile_index) const -> TileIndex
{
  if (!is_valid_index(tile_index)) {
    throw Exception {"Invalid tile index"};
  }

  const usize raw_tile_index {as_unsigned(tile_index.value)};

  // Look for cached appearance.
  if (const auto cached_appearance = mAppearanceCache[raw_tile_index];
      cached_appearance.has_value()) {
    return *cached_appearance;
  }

  // Check if the tile is animated.
  const auto& tile = mTiles[raw_tile_index];
  if (tile.is_animated()) {
    const auto& tile_animation = tile.get_animation();

    const auto current_frame_index = tile_animation.get_current_frame_index();
    const auto& current_frame = tile_animation.get_frame(current_frame_index);

    mAppearanceCache[raw_tile_index] = current_frame.tile_index;
    return current_frame.tile_index;
  }

  // No cache hit and the tile isn't animated, so just return the tile index.
  return tile_index;
}

auto Tileset::find_tile(const TileIndex tile_index) -> Tile*
{
  if (is_valid_index(tile_index)) {
    return &mTiles[usize {as_unsigned(tile_index.value)}];
  }

  return nullptr;
}

auto Tileset::find_tile(const TileIndex tile_index) const -> const Tile*
{
  if (is_valid_index(tile_index)) {
    return &mTiles[usize {as_unsigned(tile_index.value)}];
  }

  return nullptr;
}

auto Tileset::get_tile(const TileIndex tile_index) -> Tile&
{
  return *require_not_null(find_tile(tile_index), "invalid tile index");
}

auto Tileset::get_tile(const TileIndex tile_index) const -> const Tile&
{
  return *require_not_null(find_tile(tile_index), "invalid tile index");
}

auto Tileset::is_valid_index(const TileIndex tile_index) const -> bool
{
  return tile_index.value >= 0 && tile_index.value < mTileCount;
}

auto Tileset::last_tile_index() const -> TileIndex
{
  return TileIndex {narrow<TileIndex::value_type>(mTileCount - 1)};
}

auto Tileset::tile_count() const -> ssize
{
  return mTileCount;
}

auto Tileset::row_count() const -> ssize
{
  TACTILE_ASSERT(mColumnCount != 0);
  return mTileCount / mColumnCount;
}

auto Tileset::column_count() const -> ssize
{
  return mColumnCount;
}

auto Tileset::get_tile_size() const -> Int2
{
  return mTileSize;
}

auto Tileset::get_normalized_tile_size() const -> Float2
{
  return mNormalizedTileSize;
}

auto Tileset::get_texture_size() const -> Int2
{
  return mTextureSize;
}

auto Tileset::get_texture_uuid() const -> const UUID&
{
  return mTextureUuid;
}

auto Tileset::meta() -> Metadata&
{
  return mMeta;
}

auto Tileset::meta() const -> const Metadata&
{
  return mMeta;
}

}  // namespace tactile::core
