#include "remove_tileset_cmd.hpp"

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

RemoveTilesetCmd::RemoveTilesetCmd(NotNull<MapDocument*> document,
                                   const tileset_id id)
    : ACommand{"Remove Tileset"}
    , mDocument{document}
    , mId{id}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void RemoveTilesetCmd::Undo()
{
  auto& tilesets = mDocument->GetTilesets();
  tilesets.Add(mId, mTileset);
}

void RemoveTilesetCmd::Redo()
{
  auto& map = mDocument->GetMap();
  auto& tilesets = mDocument->GetTilesets();

  mTileset = tilesets.GetPtr(mId);

  const auto [first, last] = tilesets.GetRange(mId);
  map.RemoveOccurrences(first, last);

  tilesets.Remove(mId);
}

}  // namespace Tactile
