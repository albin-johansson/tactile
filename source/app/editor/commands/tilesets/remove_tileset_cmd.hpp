#pragma once

#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class remove_tileset_cmd final : public command_base
{
 public:
  remove_tileset_cmd(registry_ref registry, tileset_id id);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::remove_tileset;
  }

 private:
  registry_ref mRegistry;
  tileset_id mTilesetId;
  maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
