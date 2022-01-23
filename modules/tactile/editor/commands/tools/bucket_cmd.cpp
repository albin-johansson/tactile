#include "bucket_cmd.hpp"

#include "assert.hpp"
#include "core/algorithms/flood_fill.hpp"
#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace tactile {

BucketCmd::BucketCmd(RegistryRef registry,
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
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));

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
  TACTILE_ASSERT(entity != entt::null);
  TACTILE_ASSERT(registry.all_of<TileLayer>(entity));

  mTarget = Sys::GetTileFromLayer(registry, entity, mOrigin);
  Flood(registry, entity, mOrigin, mReplacement, mPositions);
}

}  // namespace tactile
