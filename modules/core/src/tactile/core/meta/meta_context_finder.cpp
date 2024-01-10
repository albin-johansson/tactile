// Copyright (C) 2023 Albin Johansson (GNU General Public License v3.0)

#include "tactile/core/meta/meta_context_finder.hpp"

#include "tactile/core/map/layer/group_layer.hpp"
#include "tactile/core/map/layer/object.hpp"
#include "tactile/core/map/layer/object_layer.hpp"
#include "tactile/core/map/layer/tile_layer.hpp"
#include "tactile/core/map/orthogonal_map.hpp"
#include "tactile/core/tileset/tile.hpp"
#include "tactile/core/tileset/tileset.hpp"

namespace tactile {

MetaContextFinder::MetaContextFinder(const UUID& target_uuid)
  : mTargetUuid {target_uuid}
{}

void MetaContextFinder::visit(OrthogonalMap& map)
{
  _check(map);
}

void MetaContextFinder::visit(GroupLayer& layer)
{
  _check(layer);
}

void MetaContextFinder::visit(TileLayer& layer)
{
  _check(layer);
}

void MetaContextFinder::visit(ObjectLayer& layer)
{
  _check(layer);
}

void MetaContextFinder::visit(Object& object)
{
  _check(object);
}

void MetaContextFinder::visit(Tileset& tileset)
{
  _check(tileset);
}

void MetaContextFinder::visit(Tile& tile)
{
  _check(tile);
}

void MetaContextFinder::_check(IMetaContext& context)
{
  if (!mFoundContext && context.meta().uuid() == mTargetUuid) {
    mFoundContext = &context;
  }
}

auto MetaContextFinder::found_context() -> IMetaContext*
{
  return mFoundContext;
}

}  // namespace tactile
