#pragma once

#include "core/components/texture.hpp"
#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class add_tileset_cmd final : public command_base {
 public:
  add_tileset_cmd(registry_ref registry,
                  comp::texture texture,
                  int32 tileWidth,
                  int32 tileHeight);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::add_tileset;
  }

 private:
  registry_ref mRegistry;
  comp::texture mTexture;
  int32 mTileWidth;
  int32 mTileHeight;
  maybe<tileset_id> mTilesetId;
  maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
