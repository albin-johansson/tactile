// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/map/orthogonal_map.hpp"

namespace tactile {

OrthogonalMap::OrthogonalMap(const MatrixExtent& extent, const Int2& tile_size)
  : mExtent {extent},
    mTileSize {tile_size}
{}

void OrthogonalMap::accept(IMetaContextVisitor& visitor)
{
  visitor.visit(*this);

  for (auto& attached_tileset : mTilesets) {
    attached_tileset.tileset().accept(visitor);
  }

  root_layer().each(visitor);
}

void OrthogonalMap::set_extent(const MatrixExtent& extent)
{
  mExtent = extent;
}

auto OrthogonalMap::extent() const -> const MatrixExtent&
{
  return mExtent;
}

void OrthogonalMap::set_tile_size(const Int2 size)
{
  mTileSize = size;
}

auto OrthogonalMap::tile_size() const -> Int2
{
  return mTileSize;
}

void OrthogonalMap::set_active_layer(const Maybe<UUID> layer_uuid)
{
  if (layer_uuid.has_value() && !root_layer().find_layer(*layer_uuid)) {
    mActiveLayerUuid = kNothing;
  }
  else {
    mActiveLayerUuid = layer_uuid;
  }
}

auto OrthogonalMap::active_layer_uuid() const -> Maybe<UUID>
{
  return mActiveLayerUuid;
}

auto OrthogonalMap::root_layer() -> GroupLayer&
{
  return mRootLayer;
}

auto OrthogonalMap::root_layer() const -> const GroupLayer&
{
  return mRootLayer;
}

auto OrthogonalMap::tile_format() -> TileFormat&
{
  return mTileFormat;
}

auto OrthogonalMap::tile_format() const -> const TileFormat&
{
  return mTileFormat;
}

auto OrthogonalMap::meta() -> Metadata&
{
  return mMeta;
}

auto OrthogonalMap::meta() const -> const Metadata&
{
  return mMeta;
}

}  // namespace tactile
