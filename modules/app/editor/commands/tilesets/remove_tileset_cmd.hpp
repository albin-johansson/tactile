#pragma once

#include <entt/entt.hpp>  // registry

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class RemoveTilesetCmd final : public command_base {
 public:
  RemoveTilesetCmd(registry_ref registry, tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return CommandId::RemoveTileset;
  }

 private:
  registry_ref mRegistry;
  tileset_id mTilesetId;
  maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
