#pragma once

#include <entt.hpp>  // registry

#include <tactile-base/tactile_std.hpp>

#include "tactile/core/systems/tileset_snapshot.hpp"
#include "tactile/editor/commands/command.hpp"
#include "tactile/editor/commands/command_id.hpp"

namespace Tactile {

class RemoveTilesetCmd final : public ACommand
{
 public:
  RemoveTilesetCmd(Ref<entt::registry> registry, TilesetID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  Ref<entt::registry> mRegistry;
  TilesetID mTilesetId;
  Maybe<Sys::TilesetSnapshot> mSnapshot;
};

}  // namespace Tactile
