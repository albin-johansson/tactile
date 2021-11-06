#include "set_tileset_name_cmd.hpp"

#include <cassert>  // assert
#include <utility>  // move

#include "core/components/property_context.hpp"
#include "core/systems/tileset_system.hpp"

namespace Tactile {

SetTilesetNameCmd::SetTilesetNameCmd(Ref<entt::registry> registry,
                                     const TilesetID id,
                                     std::string name)
    : ACommand{"Set Tileset Name"}
    , mRegistry{registry}
    , mTilesetId{id}
    , mNewName{std::move(name)}
{}

void SetTilesetNameCmd::Undo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindTileset(registry, mTilesetId);
  assert(entity != entt::null);

  auto& context = registry.get<PropertyContext>(entity);
  context.name = mOldName.value();
}

void SetTilesetNameCmd::Redo()
{
  auto& registry = mRegistry.get();

  const auto entity = Sys::FindTileset(registry, mTilesetId);
  assert(entity != entt::null);

  auto& context = registry.get<PropertyContext>(entity);
  mOldName = context.name;
  context.name = mNewName;
}

auto SetTilesetNameCmd::MergeWith(const ACommand& cmd) -> bool
{
  if (GetId() == cmd.GetId()) {
    const auto& other = dynamic_cast<const SetTilesetNameCmd&>(cmd);
    if (mTilesetId == other.mTilesetId) {
      mNewName = other.mNewName;
      return true;
    }
  }

  return false;
}

}  // namespace Tactile
