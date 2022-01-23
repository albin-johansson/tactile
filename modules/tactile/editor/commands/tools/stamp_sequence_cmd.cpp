#include "stamp_sequence_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace tactile {

StampSequenceCmd::StampSequenceCmd(RegistryRef registry,
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
  TACTILE_ASSERT(entity != entt::null);

  Sys::SetTilesInLayer(registry, entity, cache);
}

}  // namespace tactile
