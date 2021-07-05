#include "eraser_sequence_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

EraserSequenceCmd::EraserSequenceCmd(NotNull<MapDocument*> document,
                                     TileCache&& oldState)
    : ACommand{"Eraser Sequence"}
    , mDocument{document}
    , mOldState{std::move(oldState)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void EraserSequenceCmd::Undo()
{
  auto& map = mDocument->GetMap();

  const auto layer = map.GetActiveLayerId().value();
  auto* tileLayer = map.GetTileLayer(layer);
  assert(tileLayer);

  for (const auto& [position, tile] : mOldState)
  {
    tileLayer->SetTile(position, tile);
  }
}

void EraserSequenceCmd::Redo()
{
  auto& map = mDocument->GetMap();

  const auto layer = map.GetActiveLayerId().value();
  auto* tileLayer = map.GetTileLayer(layer);
  assert(tileLayer);

  for (const auto& [position, tile] : mOldState)
  {
    tileLayer->SetTile(position, empty_tile);
  }
}

}  // namespace Tactile
