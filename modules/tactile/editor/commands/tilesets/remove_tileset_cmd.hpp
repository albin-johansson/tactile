#pragma once

#include <tactile_def.hpp>

#include <entt/entt.hpp>  // registry

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace Tactile {

class RemoveTilesetCmd final : public ACommand {
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
