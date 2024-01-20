// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/abstract_map.hpp"

namespace tactile {

AbstractMap::AbstractMap(const MatrixExtent& extent, const Int2& tile_size)
  : mExtent {extent},
    mTileSize {tile_size}
{}

void AbstractMap::set_extent(const MatrixExtent& extent)
{
  mExtent = extent;
}

auto AbstractMap::extent() const -> const MatrixExtent&
{
  return mExtent;
}

void AbstractMap::set_tile_size(const Int2 size)
{
  mTileSize = size;
}

auto AbstractMap::tile_size() const -> Int2
{
  return mTileSize;
}

void AbstractMap::set_active_layer(const Maybe<UUID> layer_uuid)
{
  if (layer_uuid.has_value() && !root_layer().find_layer(*layer_uuid)) {
    mActiveLayerUuid = kNothing;
  }
  else {
    mActiveLayerUuid = layer_uuid;
  }
}

auto AbstractMap::active_layer_uuid() const -> Maybe<UUID>
{
  return mActiveLayerUuid;
}

auto AbstractMap::root_layer() -> GroupLayer&
{
  return mRootLayer;
}

auto AbstractMap::root_layer() const -> const GroupLayer&
{
  return mRootLayer;
}

auto AbstractMap::tile_format() -> TileFormat&
{
  return mTileFormat;
}

auto AbstractMap::tile_format() const -> const TileFormat&
{
  return mTileFormat;
}

auto AbstractMap::identifiers() -> MapIdentifiers&
{
  return mIdentifiers;
}

auto AbstractMap::identifiers() const -> const MapIdentifiers&
{
  return mIdentifiers;
}

auto AbstractMap::meta() -> Metadata&
{
  return mMeta;
}

auto AbstractMap::meta() const -> const Metadata&
{
  return mMeta;
}

}  // namespace tactile
