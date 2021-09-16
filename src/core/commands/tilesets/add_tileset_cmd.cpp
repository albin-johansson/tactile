#include "add_tileset_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/systems/tileset_system.hpp"

namespace Tactile {

AddTilesetCmd::AddTilesetCmd(Ref<entt::registry> registry,
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

  const auto entity = Sys::FindTileset(registry, id);
  assert(entity != entt::null);

  mSnapshot = Sys::CopyTileset(registry, entity);
  Sys::RemoveTileset(registry, id);
}

void AddTilesetCmd::Redo()
{
  auto& registry = mRegistry.get();
  if (!mSnapshot) {
    const auto entity = Sys::AddTileset(registry, mTexture, mTileWidth, mTileHeight);
    const auto& tileset = registry.get<Tileset>(entity);
    mTilesetId = tileset.id;
  }
  else {
    Sys::RestoreTileset(registry, *mSnapshot);
  }

  auto& active = registry.ctx<ActiveTileset>();
  active.entity = Sys::FindTileset(registry, mTilesetId.value());
}

}  // namespace Tactile
