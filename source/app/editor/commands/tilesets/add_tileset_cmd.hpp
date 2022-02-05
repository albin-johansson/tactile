#pragma once

#include <entt/entt.hpp>  // registry

#include "core/components/texture.hpp"
#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile.hpp"

namespace tactile {

class AddTilesetCmd final : public command_base {
 public:
  AddTilesetCmd(registry_ref registry,
                comp::texture texture,
                int tileWidth,
                int tileHeight);

  void undo() override;

  void redo() override;

  [[nodiscard]] auto id() const noexcept -> int override
  {
    return command_id::add_tileset;
  }

 private:
  registry_ref mRegistry;
  comp::texture mTexture;
  int mTileWidth;
  int mTileHeight;
  maybe<tileset_id> mTilesetId;
  maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
