#include "stamp_sequence_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

StampSequenceCmd::StampSequenceCmd(NotNull<MapDocument*> document,
                                   TileCache&& oldState,
                                   TileCache&& newState)
    : ACommand{"Stamp Sequence"}
    , mDocument{document}
    , mOldState{std::move(oldState)}
    , mNewState{std::move(newState)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void StampSequenceCmd::Undo()
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

void StampSequenceCmd::Redo()
{
  auto& map = mDocument->GetMap();

  const auto layer = map.GetActiveLayerId().value();
  auto* tileLayer = map.GetTileLayer(layer);
  assert(tileLayer);

  for (const auto& [position, tile] : mNewState)
  {
    tileLayer->SetTile(position, tile);
  }
}

}  // namespace Tactile
