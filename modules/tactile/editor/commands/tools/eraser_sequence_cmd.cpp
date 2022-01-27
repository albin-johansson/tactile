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
    , mLayer{sys::GetActiveLayerID(registry).value()}
    , mOldState{std::move(oldState)}
{}

void EraserSequenceCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::get_tile_layer_entity(registry, mLayer);
  auto& layer = registry.get<TileLayer>(entity);

  sys::set_tiles(layer, mOldState);
}

void EraserSequenceCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = sys::FindLayer(registry, mLayer);
  TACTILE_ASSERT(entity != entt::null);

  auto& matrix = registry.get<TileLayer>(entity).matrix;
  for (const auto& [position, _] : mOldState) {
    TACTILE_ASSERT(position.row_index() < matrix.size());
    TACTILE_ASSERT(position.col_index() < matrix.front().size());
    matrix[position.row_index()][position.col_index()] = empty_tile;
  }
}

}  // namespace tactile
