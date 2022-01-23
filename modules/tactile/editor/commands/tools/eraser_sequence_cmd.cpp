#include "eraser_sequence_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/components/layer.hpp"
#include "core/systems/layers/layer_system.hpp"
#include "core/systems/layers/tile_layer_system.hpp"

namespace tactile {

EraserSequenceCmd::EraserSequenceCmd(RegistryRef registry, TileCache&& oldState)
    : ACommand{"Eraser Sequence"}
    , mRegistry{registry}
    , mLayer{Sys::GetActiveLayerID(registry).value()}
    , mOldState{std::move(oldState)}
{}

void EraserSequenceCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayer);
  TACTILE_ASSERT(entity != entt::null);

  Sys::SetTilesInLayer(registry, entity, mOldState);
}

void EraserSequenceCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindLayer(registry, mLayer);
  TACTILE_ASSERT(entity != entt::null);

  auto& matrix = registry.get<TileLayer>(entity).matrix;
  for (const auto& [position, _] : mOldState) {
    TACTILE_ASSERT(position.GetRowIndex() < matrix.size());
    TACTILE_ASSERT(position.GetColumnIndex() < matrix.front().size());
    matrix[position.GetRowIndex()][position.GetColumnIndex()] = empty_tile;
  }
}

}  // namespace tactile
