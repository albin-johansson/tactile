#include "remove_tileset_cmd.hpp"

#include "core/systems/tileset_system.hpp"
#include "misc/assert.hpp"

namespace tactile {

RemoveTilesetCmd::RemoveTilesetCmd(registry_ref registry, const tileset_id id)
    : command_base{"Remove Tileset"}
    , mRegistry{registry}
    , mTilesetId{id}
{}

void RemoveTilesetCmd::undo()
{
  auto& registry = mRegistry.get();
  sys::restore_tileset(registry, mSnapshot.value());

  auto& active = registry.ctx<comp::active_tileset>();
  active.entity = sys::find_tileset(registry, mTilesetId);
}

void RemoveTilesetCmd::redo()
{
  const auto entity = sys::find_tileset(mRegistry, mTilesetId);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::copy_tileset(mRegistry, entity);
  sys::remove_tileset(mRegistry, mTilesetId);
}

}  // namespace tactile
