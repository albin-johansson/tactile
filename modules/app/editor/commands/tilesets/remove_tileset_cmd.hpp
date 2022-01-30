#pragma once

#include <entt/entt.hpp>  // registry

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemoveTilesetCmd final : public ACommand {
 public:
  RemoveTilesetCmd(RegistryRef registry, tileset_id id);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  RegistryRef mRegistry;
  tileset_id mTilesetId;
  maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
