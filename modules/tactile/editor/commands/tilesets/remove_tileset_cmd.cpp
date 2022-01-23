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
  sys::restore_tileset(registry, mSnapshot.value());

  auto& active = registry.ctx<ActiveTileset>();
  active.entity = sys::find_tileset(registry, mTilesetId);
}

void RemoveTilesetCmd::Redo()
{
  const auto entity = sys::find_tileset(mRegistry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::copy_tileset(mRegistry, entity);
  sys::remove_tileset(mRegistry, mTilesetId);
}

}  // namespace tactile
