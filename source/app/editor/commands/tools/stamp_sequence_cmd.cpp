#include "stamp_sequence_cmd.hpp"

#include <utility>  // move

#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

stamp_sequence_cmd::stamp_sequence_cmd(registry_ref registry,
                                       TileCache&& oldState,
                                       TileCache&& newState)
    : command_base{"Stamp Sequence"}
    , mRegistry{registry}
    , mLayer{sys::get_active_layer_id(registry).value()}
    , mOldState{std::move(oldState)}
    , mNewState{std::move(newState)}
{}

void stamp_sequence_cmd::undo()
{
  ApplySequence(mOldState);
}

void stamp_sequence_cmd::redo()
{
  ApplySequence(mNewState);
}

void stamp_sequence_cmd::ApplySequence(const TileCache& cache)
{
  auto& registry = mRegistry.get();

  const auto entity = sys::get_tile_layer_entity(registry, mLayer);
  auto& layer = registry.get<comp::tile_layer>(entity);

  sys::set_tiles(layer, cache);
}

}  // namespace tactile
