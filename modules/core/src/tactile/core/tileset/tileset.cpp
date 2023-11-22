// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/tileset/tileset.hpp"

#include "tactile/core/debug/error.hpp"
#include "tactile/core/debug/log/logger.hpp"

namespace tactile {

Tileset::Tileset(const TilesetCreateInfo& info)
  : mTileCount {info.tile_count},
    mColumnCount {info.column_count},
    mTileSize {info.tile_size},
    mTextureSize {info.texture_size},
    mNormalizedTileSize {Float2 {info.tile_size} / Float2 {info.texture_size}},
    mTextureId {info.texture_id}
{
  mTiles.reserve(static_cast<usize>(mTileCount));
  mAppearanceCache.resize(static_cast<usize>(mTileCount));

  _create_tiles();
}

void Tileset::_create_tiles()
{
  for (int32 tile_index = 0; tile_index < mTileCount; ++tile_index) {
    const auto tile_row = tile_index / mColumnCount;
    const auto tile_col = tile_index % mColumnCount;

    const Rectangle tile_region {
      .position = {tile_col * mTileSize.x, tile_row * mTileSize.y},
      .size = mTileSize,
    };

    mTiles.push_back(make_shared<Tile>(tile_region));
  }
}

void Tileset::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);
}

void Tileset::update()
{
  mAppearanceCache.assign(mAppearanceCache.size(), kNothing);

  for (auto& tile : mTiles) {
    if (tile->is_animated()) {
      auto& tile_animation = tile->get_animation();
      tile_animation.update();
    }
  }
}

auto Tileset::to_index(const TilePos& pos) const -> Maybe<TileIndex>
{
  if (pos.row >= 0 && pos.col >= 0 && pos.col < mColumnCount) {
    const auto row_count = mTileCount / mColumnCount;
    if (pos.row < row_count) {
      return TileIndex {static_cast<int32>(pos.row) * mColumnCount +
                        static_cast<int32>(pos.col)};
    }
  }

  return kNothing;
}

auto Tileset::get_appearance(const TileIndex tile_index) const -> TileIndex
{
  if (!is_valid_index(tile_index)) {
    throw RuntimeError {"Invalid tile index"};
  }

  const auto raw_tile_index = static_cast<usize>(tile_index.value);

  // Look for cached appearance.
  if (const auto cached_appearance = mAppearanceCache[raw_tile_index];
      cached_appearance.has_value()) {
    return *cached_appearance;
  }

  // Check if the tile is animated.
  const auto& tile = mTiles[raw_tile_index];
  if (tile->is_animated()) {
    const auto& tile_animation = tile->get_animation();

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
    return mTiles[static_cast<usize>(tile_index.value)].get();
  }

  return nullptr;
}

auto Tileset::find_tile(const TileIndex tile_index) const -> const Tile*
{
  if (is_valid_index(tile_index)) {
    return mTiles[static_cast<usize>(tile_index.value)].get();
  }

  return nullptr;
}

auto Tileset::get_tile(const TileIndex tile_index) -> Shared<Tile>
{
  if (is_valid_index(tile_index)) {
    return mTiles[static_cast<usize>(tile_index.value)];
  }

  throw RuntimeError {"Invalid tile index"};
}

auto Tileset::is_valid_index(const TileIndex tile_index) const -> bool
{
  return tile_index.value >= 0 && tile_index.value < mTileCount;
}

auto Tileset::tile_count() const -> int32
{
  return mTileCount;
}

auto Tileset::column_count() const -> int32
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

auto Tileset::get_texture_id() const -> TextureID
{
  return mTextureId;
}

auto Tileset::get_meta() -> Metadata&
{
  return mMeta;
}

auto Tileset::get_meta() const -> const Metadata&
{
  return mMeta;
}

}  // namespace tactile
