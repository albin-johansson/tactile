#include "stamp_sequence_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "tactile/core/systems/layers/layer_system.hpp"
#include "tactile/core/systems/layers/tile_layer_system.hpp"

namespace Tactile {

StampSequenceCmd::StampSequenceCmd(Ref<entt::registry> registry,
                                   TileCache&& oldState,
                                   TileCache&& newState)
    : ACommand{"Stamp Sequence"}
    , mRegistry{registry}
    , mLayer{Sys::GetActiveLayerID(registry).value()}
    , mOldState{std::move(oldState)}
    , mNewState{std::move(newState)}
{}

void StampSequenceCmd::Undo()
{
  ApplySequence(mOldState);
}

void StampSequenceCmd::Redo()
{
  ApplySequence(mNewState);
}

void StampSequenceCmd::ApplySequence(const TileCache& cache)
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayer);
  assert(entity != entt::null);

  Sys::SetTilesInLayer(registry, entity, cache);
}

}  // namespace Tactile
