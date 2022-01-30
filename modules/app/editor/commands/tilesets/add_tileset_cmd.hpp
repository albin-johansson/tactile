#pragma once

#include <entt/entt.hpp>  // registry

#include "core/components/texture.hpp"
#include "core/systems/snapshot.hpp"
#include "editor/commands/command.hpp"
#include "editor/commands/command_id.hpp"
#include "tactile_def.hpp"

namespace tactile {

class AddTilesetCmd final : public ACommand {
 public:
  AddTilesetCmd(RegistryRef registry,
                comp::texture texture,
                int tileWidth,
                int tileHeight);

  void Undo() override;

  void Redo() override;

  [[nodiscard]] auto GetId() const noexcept -> int override
  {
    return CommandId::AddTileset;
  }

 private:
  RegistryRef mRegistry;
  comp::texture mTexture;
  int mTileWidth;
  int mTileHeight;
  Maybe<TilesetID> mTilesetId;
  Maybe<sys::TilesetSnapshot> mSnapshot;
};

}  // namespace tactile
