#pragma once

#include <entt/entt.hpp>  // registry
#include <tactile_def.hpp>

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"

namespace tactile {

class RemoveTilesetCmd final : public ACommand {
 public:
  RemoveTilesetCmd(RegistryRef registry, TilesetID id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  RegistryRef mRegistry;
  TilesetID mTilesetId;
  Maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
