#include "add_tileset_cmd.hpp"

#include <utility>  // move

#include "assert.hpp"
#include "core/systems/tileset_system.hpp"

namespace tactile {

AddTilesetCmd::AddTilesetCmd(RegistryRef registry,
                             Texture texture,
                             const int tileWidth,
                             const int tileHeight)
    : ACommand{"Add Tileset"}
    , mRegistry{registry}
    , mTexture{std::move(texture)}
    , mTileWidth{tileWidth}
    , mTileHeight{tileHeight}
{}

void AddTilesetCmd::Undo()
{
  auto& registry = mRegistry.get();
  const auto id = mTilesetId.value();

  const auto entity = sys::FindTileset(registry, id);
  TACTILE_ASSERT(entity != entt::null);

  mSnapshot = sys::CopyTileset(registry, entity);
  sys::RemoveTileset(registry, id);
}

void AddTilesetCmd::Redo()
{
  auto& registry = mRegistry.get();
  if (!mSnapshot) {
    const auto entity = sys::MakeTileset(registry, mTexture, mTileWidth, mTileHeight);
    const auto& tileset = registry.get<Tileset>(entity);
    mTilesetId = tileset.id;
  }
  else {
    sys::RestoreTileset(registry, *mSnapshot);
  }

  auto& active = registry.ctx<ActiveTileset>();
  active.entity = sys::FindTileset(registry, mTilesetId.value());
}

}  // namespace tactile
