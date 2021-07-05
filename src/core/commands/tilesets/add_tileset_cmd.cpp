#include "add_tileset_cmd.hpp"

#include <utility>  // move

#include "core/map_document.hpp"
#include "core/tactile_error.hpp"

namespace Tactile {

AddTilesetCmd::AddTilesetCmd(NotNull<MapDocument*> document,
                             const tileset_id id,
                             Shared<Tileset> tileset)
    : ACommand{"Add Tileset"}
    , mDocument{document}
    , mId{id}
    , mTileset{std::move(tileset)}
{
  if (!mDocument)
  {
    throw TactileError{"Cannot create command from null document!"};
  }
}

void AddTilesetCmd::Undo()
{
  auto& tilesets = mDocument->GetTilesets();
  tilesets.Remove(mId);
}

void AddTilesetCmd::Redo()
{
  auto& tilesets = mDocument->GetTilesets();
  tilesets.Add(mId, mTileset);
}

}  // namespace Tactile
