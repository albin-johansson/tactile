#include "bucket_cmd.hpp"

#include <cassert>  // assert

#include "core/algorithms/flood_fill.hpp"
#include "core/components/tile_layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace Tactile {

BucketCmd::BucketCmd(Ref<entt::registry> registry,
                     const MapPosition origin,
                     const TileID replacement)
    : ACommand{"Bucket Fill"}
    , mRegistry{registry}
    , mLayer{Sys::GetActiveLayerID(registry).value()}
    , mOrigin{origin}
    , mReplacement{replacement}
{}

void BucketCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayer);
  assert(entity != entt::null);
  assert(registry.all_of<TileLayer>(entity));

  const auto target = mTarget.value();
  auto& layer = registry.get<TileLayer>(entity);
  for (const auto& position : mPositions) {
    layer.matrix.at(position.GetRowIndex()).at(position.GetColumnIndex()) = target;
  }

  mPositions.clear();
  mTarget.reset();
}

void BucketCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayer);
  assert(entity != entt::null);
  assert(registry.all_of<TileLayer>(entity));

  mTarget = Sys::GetTileFromLayer(registry, entity, mOrigin);
  FloodFill(registry, entity, mOrigin, mReplacement, mPositions);
}

}  // namespace Tactile
