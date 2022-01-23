#include "remove_tileset_cmd.hpp"

#include "assert.hpp"
#include "core/systems/tileset_system.hpp"

namespace tactile {

RemoveTilesetCmd::RemoveTilesetCmd(RegistryRef registry, const TilesetID id)
    : ACommand{"Remove Tileset"}
    , mRegistry{registry}
    , mTilesetId{id}
{}

void RemoveTilesetCmd::Undo()
{
  auto& registry = mRegistry.get();
  Sys::RestoreTileset(registry, mSnapshot.value());

  auto& active = registry.ctx<ActiveTileset>();
  active.entity = Sys::FindTileset(registry, mTilesetId);
}

void RemoveTilesetCmd::Redo()
{
  const auto entity = Sys::FindTileset(mRegistry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = Sys::CopyTileset(mRegistry, entity);
  Sys::RemoveTileset(mRegistry, mTilesetId);
}

}  // namespace tactile
